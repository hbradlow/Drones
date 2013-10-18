from SimpleCV import * 
from ticker import Ticker
import IPython

cam = Camera()
t = Ticker()

while True:
    img = cam.getImage()
    distance = img.colorDistance(color=(0,0,0))
    distance = distance.binarize(250).invert()

    blobs = distance.findBlobs()
    for b in blobs:
        if b.area() > 50 and b.area() < 1000 and b.isCircle(tolerance=.5):
            b.draw(width=5)
    #print blobs
    #if blobs: blobs.draw(width=5)

    t.tick()
    distance.show()
