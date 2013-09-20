import cv2.cv as cv
from cv import *


class Box:
    def __init__(self, x=0, y=0, w=0, h=0):
        self.x = x
        self.y = y
        self.w = w
        self.h = h

    def __mul__(self,s):
        return Box(self.x*s,self.y*s,self.w*s,self.h*s)
    __rmul__ = __mul__

    def __add__(self,box):
        return Box(self.x+box.x,self.y+box.y,self.w+box.w,self.h+box.h)

    def __repr__(self):
        return "Box(X: " + str(self.x) + " , Y: " + str(self.y) +\
                ", Width: " + str(self.w) + ", Height: " + str(self.w) + ")"


class Tracker:
    def __init__(self, video_stream, downsample=2, 
                        filter_boxes=False, filter_alpha=.5):
        self.video_stream = video_stream
        self.downsample = downsample
        self.box = Box()
        self.size = None
        self.filter_boxes = filter_boxes
        self.filter_alpha = filter_alpha

    def filter(self,box):
        """Exponential average of the stream of boxes."""
        return self.filter_alpha*box + (1-self.filter_alpha)*self.box

    def get_downsampled_frame(self):
        """Pull a frame from the video stream and downsample it."""
        img = cv.QueryFrame(self.video_stream)

        if not img:
            return None

        size = cv.GetSize(img)
        thumbnail = cv.CreateImage((size[0] / self.downsample, 
                                        size[1] / self.downsample),
                                        img.depth, 
                                        img.nChannels)
        cv.Resize(img, thumbnail)
        self.size = (size[0] / self.downsample, size[1] / self.downsample)

        return thumbnail

    def update(self):
        """Read a new image and detect the biggest pedestrian."""
        img = self.get_downsampled_frame()

        if img:
            # run the OpenCV HOG detection algorithm
            storage = CreateMemStorage(0)
            # TODO: make these numbers configurable
            found = list(HOGDetectMultiScale(img, storage, win_stride=(8,8),
                padding=(32,32), scale=1.05, group_threshold=2))

            if found:
                # pull out the biggest box
                biggest_box = max(found, key=lambda x: x[1][0]+x[1][1])
                (x, y), (w, h) = biggest_box

                if self.filter_boxes:
                    self.box = self.filter(Box(x,y,w,h))
                else:
                    self.box = Box(x,y,w,h)
