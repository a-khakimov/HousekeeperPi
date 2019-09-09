#~/usr/bin/env python

from picamera import PiCamera
from time import sleep
import datetime
import telegram
from telegram.ext import Updater
from telegram.ext import CommandHandler  


TOKEN='949089126:AAGUWleqokysidGBs8KOuMb51bPTiox154o'

reqproxy = telegram.utils.request.Request(proxy_url='socks5://98.143.145.30:62353')
bot = telegram.Bot(token=TOKEN, request=reqproxy)
print(bot.get_me())

# Getting chat id
#chat_Id = bot.get_updates()[-1].message.chat_id
#print('chat_Id: ' + str(chat_Id))

ainr_user_chat_id=174861972
bot.send_photo(chat_id=ainr_user_chat_id, photo=open('/tmp/absdiff.png', 'rb'))

import cv2
import numpy as np
from skimage.measure import compare_ssim as ssim


def mse(imageA, imageB):
    # the 'Mean Squared Error' between the two images is the
    # sum of the squared difference between the two images;
    # NOTE: the two images must have the same dimension
    err = np.sum((imageA.astype("float") - imageB.astype("float")) ** 2)
    err /= float(imageA.shape[0] * imageA.shape[1])
    return err


camera = PiCamera()
camera.resolution = (400, 400)

while False:
    FirstImgName = str(datetime.datetime.now()) + '.jpg'
    camera.capture('/tmp/' + FirstImgName)
    sleep(1)
    SecondImgName = str(datetime.datetime.now()) + '.jpg'
    camera.capture('/tmp/' + SecondImgName)

    imgA = cv2.imread('/tmp/' + FirstImgName)
    imgB = cv2.imread('/tmp/' + SecondImgName)

    imgA = cv2.cvtColor(imgA, cv2.COLOR_BGR2GRAY)
    imgB = cv2.cvtColor(imgB, cv2.COLOR_BGR2GRAY)

    absdiff = cv2.absdiff(imgA, imgB)
    cv2.imwrite("/tmp/absdiff.png", absdiff)

    diff = cv2.subtract(imgA, imgB)
    result = not np.any(diff)

    m = mse(imgA, imgB)
    s = ssim(imgA, imgB)

    print("mse: %s, ssim: %s, result: %s" % (m, s, result))

