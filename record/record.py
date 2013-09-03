import urllib2
print "Recording video..."
response = urllib2.urlopen("http://localhost:8000/dronestream")
filename = time.strftime("%Y%m%d%H%M%S",time.localtime())+".avi"
f = open("output.avi", 'wb')

video_file_size_start = 0  
video_file_size_end = 1048576 * 7  # end in 7 mb 
block_size = 1024

for i in range(9999):
    print i
    try:
        buffer = response.read(block_size)
        if not buffer:
            break
        video_file_size_start += len(buffer)
        if video_file_size_start > video_file_size_end:
            break
        f.write(buffer)

    except Exception, e:
        logger.exception(e)
f.close()
