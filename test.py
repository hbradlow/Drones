from tracker import Tracker
from ticker import Ticker
import cv2.cv as cv 

import Tkinter as tk
from gui import Visualizer, Box2D
root = tk.Tk()
vis = None

video_stream = cv.CaptureFromCAM(0)
tracker = Tracker(video_stream, filter_boxes=True, downsample=3)
ticker = Ticker()

def draw():
    global vis
    if not vis and tracker.size:
        vis = Visualizer(root, tracker.size[0], tracker.size[1])

    vis.clear()
    vis.add_drawable(Box2D([tracker.box.x,tracker.box.y],
                            [tracker.box.w,tracker.box.h]))
    vis.draw()
    root.update()

while True:
    try:
        tracker.update()
        ticker.tick()
        #print tracker.box
        if tracker.size:
            draw()

    except KeyboardInterrupt:
        break
