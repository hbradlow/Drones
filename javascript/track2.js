var arDrone = require('ar-drone');
var client  = arDrone.createClient();
var cv = require('opencv');
var ejs = require('ejs');
var http = require('http');
var url = require('url');
var fs = require('fs');
var python = require('node-python');
var process_image = python.import('process_image');

var pngStream = client.getPngStream();
var s = new cv.ImageStream();
var current_image = null;
var track = null;


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
      track = new cv.TrackedObject(current_image, [parseInt(x)-size,parseInt(y)-size,parseInt(x)+size,parseInt(y)+size], {channel: "value"});
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


s.on('data', function(mat){
    mat.save('./pic.jpg')

    process_image.test(mat);
    mat.save('./filtered.jpg')
})

pngStream.pipe(s);
