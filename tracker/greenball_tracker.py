#!/usr/bin/env python

'''
Track a green ball using OpenCV.

    Copyright (C) 2013 Simon D. Levy and Joshua Stough

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as 
    published by the Free Software Foundation, either version 3 of the 
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License 
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 You should also have received a copy of the Parrot Parrot AR.Drone 
 Development License and Parrot AR.Drone copyright notice and disclaimer 
 and If not, see 
   <https://projects.ardrone.org/attachments/277/ParrotLicense.txt> 
 and
   <https://projects.ardrone.org/attachments/278/ParrotCopyrightAndDisclaimer.txt>.
'''

import cv

# For OpenCV image display
WINDOW_NAME = 'GreenBallTracker' 

# Object-detection threshold.  You may have to adjust this (between 0 and 255) for 
# for different lighting conditions.
THRESHOLD = 100

def track(img_bytes, img_width, img_height):
    '''Accepts BGR image bytes, image width, and image height. 
           Returns: (x,y) coordinates of centroid if found
                    None if no centroid was found
                    (0,0) if user hit ESC
    '''

    # Create full-color image from bytes
    full_image = _create_image_header(img_width, img_height, 3)  
    cv.SetData(full_image, img_bytes, img_width*3)
  
    # Create separate red, green, and blue images from bytes
    r_image = _create_grayscale_image(img_bytes, img_width, img_height, 2)
    b_image = _create_grayscale_image(img_bytes, img_width, img_height, 0)
    g_image = _create_grayscale_image(img_bytes, img_width, img_height, 1)

    # Remove 1/3 of red and blue components from green
    threes_image = cv.CreateImage((img_width,img_height), cv.IPL_DEPTH_8U, 1)  
    cv.Set(threes_image, 3)
    _div_and_sub(g_image, r_image, threes_image)
    _div_and_sub(g_image, b_image, threes_image)

    # Threshold and erode green image
    cv.Threshold(g_image, g_image, THRESHOLD, 255, cv.CV_THRESH_BINARY)
    cv.Erode(g_image, g_image)

    # Find centroid of eroded image
    moments = cv.Moments(cv.GetMat(g_image), 1) # binary flag
    centroid_x = _centroid(moments, 1, 0)
    centroid_y = _centroid(moments, 0, 1)

    # Assume no centroid
    ctr = None

    # Use centroid if it exists
    if centroid_x != None and centroid_y != None:

        ctr = (centroid_x, centroid_y)

        # Put black circle in at centroid in image
        cv.Circle(full_image, ctr, 4, (0,0,0))

    # Display full-color image
    cv.NamedWindow(WINDOW_NAME)
    cv.ShowImage(WINDOW_NAME, full_image)

    # Force image display, setting centroid to (0,0) on ESC key input
    if cv.WaitKey(5) == 27:
        ctr = (0,0)
    
    # Return coordinates of centroid
    return ctr

# Removes 1/3 of other_image from src_dst_image
def _div_and_sub(src_dst_image, other_image, threes_image):
    cv.Div(other_image, threes_image, other_image) 
    cv.Sub(src_dst_image, other_image, src_dst_image) 

# Computes centroid based on x, y flags
def _centroid(moments, x, y):
    result = None
    moments00 = cv.GetSpatialMoment(moments, 0, 0)
    if moments00 != 0:
        result = int(cv.GetSpatialMoment(moments, x, y) / moments00)
    return result

# Creates grayscale image from bytes
def _create_grayscale_image(img_bytes, img_width, img_height, channel):
    image = _create_image_header(img_width, img_height, 1)
    cv.SetData(image, img_bytes[channel::3], img_width)
    return cv.GetMat(image)

# Creates image header from bytes
def _create_image_header(img_width, img_height, channels):
    return cv.CreateImageHeader((img_width,img_height), cv.IPL_DEPTH_8U, channels)  
    
    
# Test with input from camera
if __name__ == '__main__':

    capture = cv.CaptureFromCAM(0)

    while True:
	
        image = cv.QueryFrame(capture)
    
        if track(image.tostring(), image.width, image.height)  == (0,0):
            break
