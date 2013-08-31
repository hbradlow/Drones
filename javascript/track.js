var arDrone = require('ar-drone');
var client  = arDrone.createClient();
var cv = require('opencv');
var ejs = require('ejs');
var http = require('http');
var url = require('url');
var fs = require('fs');
var sys = require('sys')
var exec = require('child_process').exec;

var pngStream = client.getPngStream();
var s = new cv.ImageStream();
var current_image = null;
var track = null;
var track_window = null;
var color = null;
var counter = 0;


http.createServer(function(req, res){
  var request = url.parse(req.url, true);
  var action = request.pathname;

  if (action == '/picker_image') {
     var img = fs.readFileSync('./pic.jpg');
     res.writeHead(200, {'Content-Type': 'image/jpg' });
     res.end(img, 'binary');
  }
  if (action == '/jquery.min.js') {
     var js = fs.readFileSync('./jquery.min.js');
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end(js);
    }
  if (action == '/set_window'){
      var size = 10;
      var x = request.query.x;
      var y = request.query.y;
       track_window = [parseInt(x)-size,parseInt(y)-size,parseInt(x)+size,parseInt(y)+size];
      current_image = current_image.roi(parseInt(x)-size,parseInt(y)-size,size*2,size*2);
        current_image.save('./roi.jpg')
        var mean = current_image.meanStdDev().mean;
      color = [mean.get(0,0),mean.get(1,0),mean.get(2,0)];
        console.log(color);

      //track = new cv.TrackedObject(current_image, [parseInt(x)-size,parseInt(y)-size,parseInt(x)+size,parseInt(y)+size], {channel: "value"});
  }
   else { 
       var str = fs.readFileSync('./picker.html', 'utf8');
       var ret = ejs.render(str, {
             names: ['foo', 'bar', 'baz']
       });
         res.writeHead(200);
       res.end(str);
    }
}).listen(8000, '127.0.0.1');

function getMethods(obj) {
  var result = [];
  for (var id in obj) {
    try {
      if (typeof(obj[id]) == "function") {
        result.push(id + ": " + obj[id].toString());
      }
    } catch (err) {
      result.push(id + ": inaccessible");
    }
  }
  return result;
}

s.on('data', function(mat){
    if(current_image==null)
        console.log("Running...");
    var first = mat.copy();
    mat.save('./pic.jpg')
    current_image = mat;

    if(color!=null){
        var diff = 30;
        var lower = [color[0]-diff,color[1]-diff,color[2]-diff];
        var upper = [color[0]+diff,color[1]+diff,color[2]+diff];
        mat.inRange(lower,upper);
        mat.erode(5);

        /*
        var count = 0;
        var x_total = 0;
        var y_total = 0;
        for(var i = 0; i<mat.width(); i+=5){
            for(var j = 0; j<mat.height(); j+=5){
                if(mat.get(j,i)<0 || mat.get(j,i)>0){
                    //console.log(i+ "," + j);
                    count += 1;
                    x_total += i;
                    y_total += j;
                }
            }
        }

        var x_ave = x_total/count;
        var y_ave = y_total/count;
        //console.log("X: " + x_ave + ",Y: " + y_ave);
        first.rectangle([x_ave-20, y_ave-20], [x_ave+20, y_ave+20])
        first.save("./saved.jpg");
        //track = new cv.TrackedObject(mat, track_window, {channel: "value"});
        */
    }

    /*
    var err = {};
    if(track!=null){
        var rec = track.track(mat);
        console.log(rec);
        mat.rectangle([rec[0], rec[1]], [rec[2], rec[3]])
    }
    */

    mat.save('./filtered.jpg')


    counter += 1;
    if(color!=null && counter%5==0){
        function puts(error, stdout, stderr) { 
            if(stdout.trim()){
                var objs = JSON.parse(stdout);
                console.log(objs);
                for(var i = 0; i<objs.length; i++){
                    var obj = objs[i];
                    first.rectangle([obj[0],obj[1]],[obj[0]+obj[2],obj[1]+obj[3]]);
                }
                first.save("./saved.jpg");
            }
        }
        exec("python process_image.py filtered.jpg", puts);
    }
})

pngStream.pipe(s);
