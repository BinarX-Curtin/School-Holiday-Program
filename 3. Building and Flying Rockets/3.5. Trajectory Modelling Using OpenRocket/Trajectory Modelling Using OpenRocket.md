# Payload Section Design in 3D CAD <!-- omit from toc -->

## NOTES
Kyle said the height of the rocket with camera was modelled to 240ish feet approximately 80m. MD for that session with only the order of operations. Would be good to use CLIPCAM to record the rest of that session with Kyle showing on the screen in a hands on demo (Mic'd up with just screen or with webcam).

### Table of Contents <!-- omit from toc -->
- [NOTES](#notes)
- [Objectives](#objectives)
- [Requirements](#requirements)
- [Resources](#resources)
- [Lesson Plan](#lesson-plan)
- [Procedure](#procedure)
- [1.0 Introduction to Trajectory Modelling ](#10-intro-to-trajectory-modelling)
- [2.0 Introduction to OpenRocket UI](#20-intro-to-openrocket-ui)
- [3.0 Open Rocket Sequence of Operations](#30-open-rocket-sequence-of-operations)
- [4.0 Flight Simulation and Data Analysis](#40-flight-simulation-and-data-analysis)

## Objectives
1. Introduce students to OpenRocket
2. Add the measured dimensions of the rocket to OpenRocket
3. Model the trajectory of the rocket using OpenRocket

## Requirements
1. OpenRocket installed on device
2. Rocket models
3. Payloads

## Resources
- OpenRocket User Guide: https://wiki.openrocket.info/User%27s_Guide
- Civil Aviation Safety Regulations: https://www.legislation.gov.au/F1998B00220/latest/text

## Session Plan
    Session Start                                       - 0930
Introduction to trajectory modelling                    - 0930-0940
Introduction OpenRocket UI                              - 0940-1000
Input rocket into OpenRocket                            - 1000-1045
Flight simulation and data analysis                     - 1045-1055
Close out                                               - 1055-1100
    Session End                                         - 1100 

## Procedure
## 1.0 Introduction to Trajectory Modelling 

During this section, the students should consider the following questions:

1. What are the benefits of simulation and modelling?
    - What are the benefits of doing practical testing?
2. What is needed to do a simulation?
3. What are the system (rocket and environment) parameters that affect the behaviour?

## 2.0 Introduction to OpenRocket UI

In this section, pay attention to how a rocket is inputted into OpenRocket, and at try it out yourself.
Attempt any of the following challenges:

1. Can you cause your rocket to crash?
2. Can you get your rocket into space?
3. How far from the launch site can you get the rocket?
4. How close to the launch site can you get it to land?
5. Where is the best launch site? (Consider effects of gravity)
6. How long can you get your rocket to stay in the air?

Remember, a good rocket will have a stability number between 1 and 2.
Anything over that is unnecessarily stable. Anything under that is unstable.

## 3.0 Open Rocket Sequence of Operations

The students should input the measured parameters of each component of the rocket
from nosecone to fins, then add the payload, and finally the motor.
Expected parameters will be provided on the powerpoint if you get stuck.

For simplicity, it should be in the following order:

1. Nose Cone - Nose Cone
    - Approximate shape from given options
    - Input measured length and diameter 
    - Input wall thickness
    - Input material
2. Payload Section - Body Tube
    - Input measured length and diameter 
    - Input wall thickness
    - Input material
3. Body - Body Tube
    - Input measured length and diameter 
    - Input wall thickness
    - Input material
    3.1 Fins - Freeform
        - Input shape
        - Input thickness
        - Input material
    3.2 Engine Mount Tube
        - Input measured length and diameter 
        - Input wall thickness
        - Input material
4. Additional Masses
    4.1 Parachute - Parachute
        - Add to Body
        - Find parachute model
    4.2 Payload - Mass
        - Add to Payload Section
        - Input measured mass
    4.3 Camera - Pod + Body Tube
        - Input measured mass
        - Position correctly
5. Motor - Motor
    - Select motor from list
6. Extension - For additional realism
    - Shock Cord
    - Launch Lug
    - Detailed Engine Mount

## 4.0 Flight Simulation and Data Analysis

From your simulated results the should answer the following questions:

1. Did it crash?
2. How high did it go?
    - Is this allowed? (See Civil Aviation Safety Regulations if to try answer this yourselves)
3. How fast did the rocket go?
4. How long will it be in the air?
5. How fast will it hit the ground?
6. How far away will it land?
7. Are any changes needed?