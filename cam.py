import cv2.cv as cv
import cv2
from cv import *
import time

import Tkinter as tk
from gui import Visualizer, Box2D
root = tk.Tk()
vis = None

Capture = cv.CaptureFromCAM(0)

class Ticker:
    def __init__(self):
        self.previous_time = time.time()
        self.frequency = 0
        self.alpha = .5

        self.print_cycle = 5
        self.count = 0

    def tick(self,quite=False):
        self.count += 1

        now = time.time()
        diff = now-self.previous_time
        self.previous_time = now

        new_frequency = 1.0/diff
        self.frequency = self.alpha*new_frequency + (1-self.alpha)*self.frequency

        if self.count % self.print_cycle == 0:
            if not quite:
                print "Frequency",self.frequency,"Hz"
            return True

        return False

ticker = Ticker()
images = []
try:
    while True:
        Img = cv.QueryFrame(Capture)
        downsample = 2

        size = cv.GetSize(Img)
        thumbnail = cv.CreateImage( ( size[0] / downsample, size[1] / downsample), Img.depth, Img.nChannels)
        cv.Resize(Img, thumbnail)

        if not vis:
            vis = Visualizer(root, size[0] / downsample, size[1] / downsample)
            vis.run()


        storage = CreateMemStorage(0)
        found = list(HOGDetectMultiScale(thumbnail, storage, win_stride=(8,8),
            padding=(32,32), scale=1.05, group_threshold=2))

        print found

        if found:
            vis.clear()
        for r in found:
            (rx, ry), (rw, rh) = r
            vis.add_drawable(Box2D([rx,ry],[rw,rh]))
            tl = (rx + int(rw*0.1), ry + int(rh*0.07))
            br = (rx + int(rw*0.9), ry + int(rh*0.87))
            Rectangle(thumbnail, tl, br, (0, 255, 0), 3)

        vis.draw()
        root.update()
        images.append(thumbnail)

        if ticker.tick(quite=False):
            pass
            #ShowImage("test",thumbnail)
except KeyboardInterrupt:
    pass

exit()
for index,image in enumerate(images):
    SaveImage("output/image_" + str(index) + ".png",image)
