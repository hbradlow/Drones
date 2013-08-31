var autonomy = require('ardrone-autonomy');
var mission  = autonomy.createMission();

mission.takeoff()
       .zero()       // Sets the current state as the reference
       .altitude(1)  // Climb to altitude = 1 meter
       .forward(1)   
       .right(1)     
       .backward(1) 
       .left(1)
       .hover(1000)  // Hover in place for 1 second
       .land();

mission.run(function (err, result){
    if (err) {
        console.trace("Oops, something bad happened: %s", err.message);
        mission.client().stop();
        mission.client().land();
    } else {
        console.log("Mission success!");
        process.exit(0);
    }
});
