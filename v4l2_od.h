/* The device has capability of object detection */
#define V4L2_CAP_OBJ_DETECTION		0x00100000

#define VIDIOC_G_OD_COUNT	_IOWR('V', 98, struct v4l2_od_count)
#define VIDIOC_G_OD_RESULT	_IOWR('V', 99, struct v4l2_od_result)

#define __u8 uint8_t
#define __u16 uint16_t
#define __u32 uint32_t

struct v4l2_od_obj_desc {
	__u16		centerx;
	__u16		centery;
	__u16		sizex;
	__u16		sizey;
	__u16		angle;
	__u16		reserved[5];
} v4l2_od_obj_desc;

struct v4l2_od_face_desc {
	__u16	id;
	__u8	smile_level;
	__u8    reserved[15];

	struct v4l2_od_obj_desc	f;
};

struct v4l2_od_eye_desc {
	__u16	face_id;
	__u8	blink_level;
	__u8    reserved[15];

	struct v4l2_od_obj_desc	e;
};

struct v4l2_od_mouth_desc {
	__u16	face_id;
	__u8    reserved[16];

	struct v4l2_od_obj_desc	m;
};

enum v4l2_od_type {
	V4L2_OD_TYPE_FACE		= 1,
	V4L2_OD_TYPE_LEFT_EYE		= 2,
	V4L2_OD_TYPE_RIGHT_EYE		= 3,
	V4L2_OD_TYPE_MOUTH		= 4,
	V4L2_OD_TYPE_USER_DEFINED	= 255,
	V4L2_OD_TYPE_MAX_CNT		= 256,
};

struct v4l2_od_object {
	enum v4l2_od_type	type;
	__u16			confidence;
	union {
		struct v4l2_od_face_desc face;
		struct v4l2_od_face_desc eye;
		struct v4l2_od_face_desc mouth;
		__u8	rawdata[60];
	} o;
};

struct v4l2_od_result {
	__u32			frm_seq;
	__u32			obj_cnt;
	__u32			reserved[6];
	struct v4l2_od_object	od[0];
};

struct v4l2_od_count {
	__u32	frm_seq;
	__u32	obj_cnt;
	__u32	reserved[6];
};

