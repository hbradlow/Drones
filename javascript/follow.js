var cv = require('opencv');
var ejs = require('ejs');
var http = require('http');
var url = require('url');
var fs = require('fs');
var sys = require('sys')
var exec = require('child_process').exec;
var pid = require('./pid.js');
//set up video on port 5000
require('ar-drone-png-stream')(client, { port: 5000 });

var pngStream = pid.client.getPngStream();
var s = new cv.ImageStream();
var current_image = null;
var color = null;
var counter = 0;

var safety = false;
var output = null;
var current = null;


http.createServer(function(req, res){
  var request = url.parse(req.url, true);
  var action = request.pathname;

  if (action == '/filtered_image') {
     var img = fs.readFileSync('./filtered.jpg');
     res.writeHead(200, {'Content-Type': 'image/jpg' });
     res.end(img, 'binary');
  }
  if (action == '/output_image') {
     var img = fs.readFileSync('./output.jpg');
     res.writeHead(200, {'Content-Type': 'image/jpg' });
     res.end(img, 'binary');
     console.log("output_image");
  }
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
    if (action == '/track'){
        var x = request.query.x;
        var y = request.query.y;
        if(safety){
            pid.track([x,y]);
        }
    }
  if (action == '/set_window'){
      var size = 10;
      var x = request.query.x;
      var y = request.query.y;
      current_image = current_image.roi(parseInt(x)-size,parseInt(y)-size,size*2,size*2);
        current_image.save('./roi.jpg')
        var mean = current_image.meanStdDev().mean;
      color = [mean.get(0,0),mean.get(1,0),mean.get(2,0)];
        console.log(color);

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

if(safety){
    pid.client.takeoff();
}
/*

s.on('data', function(mat){
    if(current_image==null)
        console.log("Running...");
    output = mat.copy();
    mat.save('./pic.jpg')
    current_image = mat;

    if(color!=null){
        var diff = 30;
        var lower = [color[0]-diff,color[1]-diff,color[2]-diff];
        var upper = [color[0]+diff,color[1]+diff,color[2]+diff];
        mat.inRange(lower,upper);
        mat.erode(5);

    mat.save('./filtered.jpg')


    counter += 1;
    if(color!=null && counter%5==0){
        function puts(error, stdout, stderr) { 
            if(stdout.trim()){
                /*
                var objs = JSON.parse(stdout);
                console.log(objs);
                for(var i = 0; i<objs.length; i++){
                    var obj = objs[i];
                    first.rectangle([obj[0],obj[1]],[obj[0]+obj[2],obj[1]+obj[3]]);
                }
                first.save("./saved.jpg");
                */
                var center = JSON.parse(stdout);
                if(safety){
                    pid.track(center);
                }
                var size = 20;
                output.rectangle([center[0]-size,center[1]-size],
                                [center[0]+size,center[1]+size]);
                output.save("output.jpg");
                console.log(center);
            }
        }
        exec("python process_image.py filtered.jpg", puts);
    }
    }
})
/*

pngStream.pipe(s);
