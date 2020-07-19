# Digital-traffic-light
Digital traffic light using Tiva C

## Requirments
* Tiva C TM4C123G LaunchPad Evaluation Kit
* Code Composer studio
# Description
### System outputs:
-	Three LEDs connected to ground.
-	Red LED is connected to PORTF pin number 1.
-	Blue LED is connected to PORTF pin number 2.
-	Green LED is connected to PORTF pin number 3.
### System States:
-	<strong> Cars passing state: </strong> where cars are allowed to pass and the Traffic light is RED.
  -	<strong>Pedestrians passing state: </strong> where pedestrians are allowed to pass and the traffic light is GREEN.
  -	<strong> Pedestrians waiting state:</strong> where the pedestrians are waiting to cross the road.
### State transitions:
-	By default, system shall be on the Car passing state, RED light is blinking.
-	After a button press, system shall switch to Pedestrians waiting state.
-	In the Pedestrians waiting state, system shall blink the yellow light.
-	In the Pedestrians waiting state, button presses shall be ignored.
-	After the 10 seconds passed, system shall switch to the Pedestrians passing state.
-	In the Pedestrians passing state, system shall blink the green light.
-	In the Pedestrians passing state, button presses shall be ignored.
-	After the 30 seconds passed, system shall switch to the car passing state.

