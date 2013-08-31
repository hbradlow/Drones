var arDrone = require('ar-drone');
var client  = arDrone.createClient();
var cv = require('opencv');

var pngStream = client.getPngStream();
var s = new cv.ImageStream()

var lower_threshold = [0, 0, 80];
var upper_threshold = [100, 100, 120];

var match = null;
s.on('data', function(mat){
    mat.save('./pic.jpg')
    /*
    //Face detect
    mat.detectObject(cv.FACE_CASCADE, [], function(err, matches){
        //console.log(matches);
        for(var i = 0; i<matches.length; i++){
            match = matches[i];
        }
    });
    if(match!=null)
    {
        console.log(match);
        mat.ellipse(match.x,match.y);
    }
    */
    /*
    //Edge detect
    mat.convertGrayscale()
    mat.canny(5, 300)
    mat.houghLinesP()
    */
    //Color filter
    mat.inRange(lower_threshold, upper_threshold);
    mat.erode(15);
    mat.save('./filtered.jpg')
})

pngStream.pipe(s);
