#!/bin/sh

function helper {
echo "*******************************************************"
echo "Usage for run.sh [-d input -e engine -h -o output -u width -v height]"
echo "All arguments except f are passed to aicv"
echo "d:		Select input. csi2b-NV12: 0, csi2b-Y8: 1, webcam-YUYV: 2 (default: 2)"
echo "e engine:	Select engine. none: 0, canny: 1, hog: 2, lkt: 3, face: 4, headband: 5 (default: 0)"
echo "f fps:		Set fps (passed to setup.sh)"
echo "h:		Print this usage, usage of aicv and exit"
echo "o output:	Select output. printf: 1, video: 2, memory: 4 (default: 7)"
echo "u width:		Set width (default: 320)"
echo "v height:	Set height (default: 240)"
SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`
$SCRIPTPATH/aicv -h
exit 0
}

INPUT=2
ENGINE=0
OUTPUT=7
WIDTHB=320
HEIGHTB=240
while getopts d:e:f:ho:u:v: opt
do
	case "$opt" in
		d) INPUT="$OPTARG";;
		e) ENGINE="$OPTARG";;
		f) FPS="$OPTARG";;
		h) helper;;
		o) OUTPUT="$OPTARG";;
		u) WIDTHB="$OPTARG";;
		v) HEIGHTB="$OPTARG";;
	esac
done

SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`
gst-inspect | grep -q ducatih264enc
if [ $INPUT = 0 -o $INPUT = 1 ]; then
	if [ ! -z $FPS ]; then
		SETUPOPTION="$SETUPOPTION -f $FPS"
	fi
	SETUPOPTION="$SETUPOPTION -u $WIDTHB -v $HEIGHTB"
	if [ $INPUT = 0 ]; then
		SETUPOPTION="$SETUPOPTION -c"
	fi
	/home/ai/aiGstRtspServer/setup.sh $SETUPOPTION
fi
if [ $OUTPUT = 2 -o $OUTPUT = 3 -o $OUTPUT = 7 ]; then
	pgrep X
	if [ $? != 0 ]; then
		echo "Starting X now"
		X &
		sleep 2
		export DISPLAY=:0.0
		xset -dpms s off
	fi
	export DISPLAY=:0.0
fi
OPTION="-d $INPUT -e $ENGINE -o $OUTPUT -u $WIDTHB -v $HEIGHTB"
$SCRIPTPATH/aicv $OPTION

#gst-launch v4l2src device=/dev/video6 ! "video/x-raw-yuv,format=(fourcc)YUY2,width=320,height=240,framerate=30/1" ! ffmpegcolorspace ! dri2videosink
#gst-launch v4l2src queue-size=8 device=/dev/video5 ! "video/x-raw-gray,bpp=(int)8,width=320,height=240,framerate=30/1" ! ffmpegcolorspace ! dri2videosink

