from SimpleCV import * 
from ticker import Ticker
import IPython

cam = Camera()
t = Ticker()

def maxValue(self,locations=False):
    if(locations):
        val = np.max(self.getGrayNumpy())
        x,y = np.where(self.getGrayNumpy()==val)
        locs = zip(x.tolist(),y.tolist())
        return int(val),locs
    else:
        val = np.max(self.getGrayNumpy())
        return int(val)

while True:
    img = cam.getImage()
    gray = img.grayscale()
    red = img.splitChannels()[0]
    subtracted = red-gray
    subtracted *= 255/maxValue(subtracted)
    binary = subtracted.binarize(200).invert()

    blobs = binary.findBlobs()
    if blobs:
        total_area = 0
        centroid_x = 0
        centroid_y = 0
        for b in blobs:
            centroid_x += b.area()*b.centroid()[0]
            centroid_y += b.area()*b.centroid()[1]
            total_area += b.area()
        centroid_x /= total_area
        centroid_y /= total_area
        binary.drawCircle((centroid_x,centroid_y),10,color=Color.GREEN)

    t.tick()
    binary.show()