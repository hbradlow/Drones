var sys = require('sys')
var exec = require('child_process').exec;
var test = null;
function puts(error, stdout, stderr) { 
    test = stdout; 
    console.log("Done!");
    console.log(test);
}
exec("python process_image.py", puts);
