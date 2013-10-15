from SimpleCV import * 
from ticker import Ticker

cam = Camera()
t = Ticker()

while True:
    img = cam.getImage()
    distance = img.colorDistance(color=(0,0,0))
    distance = distance.binarize(250).invert()

    blobs = distance.findBlobs()
    #print blobs
    if blobs: blobs.draw(width=5)

    t.tick()
    distance.show()
