#~/usr/bin/env python

from time import sleep
import datetime
import telegram
from telegram.ext import Updater
from telegram.ext import CommandHandler
import sys
from PIL import Image
import cv2
import numpy as np
from skimage.measure import compare_ssim as ssim
import shutil
import requests

TOKEN='949089126:AAGUWleqokysidGBs8KOuMb51bPTiox154o'

#reqproxy = telegram.utils.request.Request(proxy_url='socks5://190.6.196.118:9999')
bot = telegram.Bot(token=TOKEN)
print(bot.get_me())

# Getting chat id
#chat_Id = bot.get_updates()[-1].message.chat_id
#print('chat_Id: ' + str(chat_Id))
ainr_user_chat_id=174861972

def mse(imageA, imageB):
    # the 'Mean Squared Error' between the two images is the
    # sum of the squared difference between the two images;
    # NOTE: the two images must have the same dimension
    err = np.sum((imageA.astype("float") - imageB.astype("float")) ** 2)
    err /= float(imageA.shape[0] * imageA.shape[1])
    return err



def get_image(host, img):
	response = requests.get(host, stream=True)
	with open(img, 'wb') as out_file:
		shutil.copyfileobj(response.raw, out_file)
	del response


while True:
    get_image('http://localhost:2080/image', '/tmp/img1.png')
    sleep(1)
    get_image('http://localhost:2080/image', '/tmp/img2.png')

    imgA = cv2.imread('/tmp/img1.png')
    imgB = cv2.imread('/tmp/img2.png')

    imgA = cv2.cvtColor(imgA, cv2.COLOR_BGR2GRAY)
    imgB = cv2.cvtColor(imgB, cv2.COLOR_BGR2GRAY)

    absdiff = cv2.absdiff(imgA, imgB)
    cv2.imwrite("/tmp/absdiff.png", absdiff)

    diff = cv2.subtract(imgA, imgB)
    result = not np.any(diff)

    m = mse(imgA, imgB)
    s = ssim(imgA, imgB)
    
    msg = "mse: " + str(m) + ", ssim: " + str(s) + ", result: " + str(result)

    print(msg)
    
    if m > 50:
        imgs_path = ['/tmp/img1.png',
                     '/tmp/img2.png',
                     '/tmp/absdiff.png']
   
        total_width = 0
        max_height = 0
    
        for img_path in imgs_path:
            img = Image.open(img_path)
            widths, heights = img.size
            total_width += widths
            max_height = heights

        new_im = Image.new('RGB', (total_width, max_height), 0)

        x_offset = 0
        for img_path in imgs_path:
            im = Image.open(img_path)
            new_im.paste(im, (x_offset, 0))
            x_offset += im.size[0]
            new_im.save('/tmp/test.png', 'PNG')
    
        bot.send_photo(chat_id=ainr_user_chat_id, photo=open('/tmp/test.png', 'rb'))
        bot.send_message(chat_id=ainr_user_chat_id, text=msg)
