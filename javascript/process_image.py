from SimpleCV import Image
import argparse
import json

def center(filename):
    image = Image(filename)
    blobs = image.findBlobs()
    box = blobs[0].mBoundingBox
    x = box[0]+box[2]/2.
    y = box[1]+box[3]/2.
    return [x,y]

def blobs(filename):
    image = Image(filename)
    blobs = image.findBlobs()
    return json.dumps([b.mBoundingBox for b in blobs])

if __name__=="__main__":
    parser = argparse.ArgumentParser(description='Process some integers.')
    parser.add_argument('filename', type=str,
                               help='filename of image to be processed')
    args = parser.parse_args()
    print center(args.filename)
