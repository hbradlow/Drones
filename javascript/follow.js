var cv = require('opencv');
var ejs = require('ejs');
var http = require('http');
var url = require('url');
var fs = require('fs');
var sys = require('sys')
var exec = require('child_process').exec;
var pid = require('./pid.js');
var drone_video = require('./node-drone-video/bin/drone-video');
//set up video on port 5000
//require('ar-drone-png-stream')(pid.client, { port: 5000 });

//var pngStream = pid.client.getPngStream();
var s = new cv.ImageStream();
var current_image = null;
var color = null;
var counter = 0;

var safety = true;
var output = null;
var current = null;


var server = http.createServer(function(req, res){
  var request = url.parse(req.url, true);
  var action = request.pathname;

  if (action == '/filtered_image') {
     var img = fs.readFileSync('./filtered.jpg');
     res.writeHead(200, {'Content-Type': 'image/jpg' });
     res.end(img, 'binary');
  }
  else if (action == '/output_image') {
     var img = fs.readFileSync('./output.jpg');
     res.writeHead(200, {'Content-Type': 'image/jpg' });
     res.end(img, 'binary');
     console.log("output_image");
  }
  else if (action == '/picker_image') {
     var img = fs.readFileSync('./pic.jpg');
     res.writeHead(200, {'Content-Type': 'image/jpg' });
     res.end(img, 'binary');
  }
  else if (action == '/jquery.min.js') {
     var js = fs.readFileSync('./jquery.min.js');
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end(js);
    }
      else if (action == '/video_stream'){
        console.log("video!");
      require("fs").createReadStream(__dirname + "/dronestream.html").pipe(res);
    }
    else if (action == '/track'){
        var x = request.query.x;
        var y = request.query.y;
        console.log(x + "," + y);
        if(safety){
            pid.track([x,y]);
        }
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end("//nothing");
    }
      else if (action == '/takeoff'){
          console.log("takeoff");
          if(safety){
            pid.client.takeoff();
          }
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end("//nothing");
      }
      else if (action == '/stop'){
          console.log("stop");
            pid.client.stop();
            safety = false;
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end("//nothing");
      }
      else if (action == '/back'){
          console.log("back");
          if(safety){
            pid.back(.1);
        }
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end("//nothing");
      }
      else if (action == '/pause'){
          console.log("stop");
            pid.stop();
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end("//nothing");
      }
      else if (action == '/forward'){
          console.log("forward");
          if(safety){
            pid.forward(.1);
        }
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end("//nothing");
      }
      else if (action == '/land'){
          console.log("land");
            pid.client.land();
            shutdown();
            safety = false;
         res.writeHead(200, {'Content-Type': 'text/javascript' });
       res.end("//nothing");
      }
    else if (action == '/set_window'){
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
});


var record_functions = drone_video.record(pid.client);
var shutdown = record_functions[0];
var parser = record_functions[1];

require("./node-dronestream/lib/server").listen(server,{},parser); 
server.listen(8000, '127.0.0.1');

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
*/

//pngStream.pipe(s);
