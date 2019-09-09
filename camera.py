#~/usr/bin/env python

from picamera import PiCamera
from time import sleep

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



# return the MSE, the lower the error, the more "similar"
    # the two images are

camera = PiCamera()
camera.resolution = (500, 500)

#imgA = np.empty((500, 500, 3), dtype=np.uint8)
#imgB = np.empty((500, 500, 3), dtype=np.uint8)

camera.capture('/tmp/pic1.jpg')
#camera.capture(imgA, 'bgr')
sleep(1)
camera.capture('/tmp/pic2.jpg')
#camera.capture(imgB, 'bgr')

imgA = cv2.imread("/tmp/pic1.jpg")
imgB = cv2.imread("/tmp/pic2.jpg")

#print(type(imgA))
#print(type(imgB))


imgA = cv2.cvtColor(imgA, cv2.COLOR_BGR2GRAY)
imgB = cv2.cvtColor(imgB, cv2.COLOR_BGR2GRAY)

absdiff = cv2.absdiff(imgA, imgB)
cv2.imwrite("/tmp/absdiff.png", absdiff)

diff = cv2.subtract(imgA, imgB)
result = not np.any(diff)

m = mse(imgA, imgB)
s = ssim(imgA, imgB)

print("mse: %s, ssim: %s" % (m, s))

if result:
    print("The images are the same")
else:
    cv2.imwrite("/tmp/diff.png", diff)
    print("The images are different")

#camera = PiCamera()
#camera.start_preview()

#sleep()

#camera.capture('/tmp/pic1.jpg')
#sleep(10)
#camera.capture('/tmp/pic2.jpg')
#camera.stop_preview()
