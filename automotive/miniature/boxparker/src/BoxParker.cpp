/**
 * boxparker - Sample application for realizing a box parking car.
 * Copyright (C) 2012 - 2015 Christian Berger
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <iostream>

#include "opendavinci/odcore/data/Container.h"

#include "opendavinci/GeneratedHeaders_OpenDaVINCI.h"
#include "automotivedata/GeneratedHeaders_AutomotiveData.h"

#include "BoxParker.h"

namespace automotive {
    namespace miniature {

        using namespace std;
        using namespace odcore::base;
        using namespace odcore::base::module;
        using namespace odcore::data;
        using namespace automotive;

        BoxParker::BoxParker(const int32_t &argc, char **argv) :
            TimeTriggeredConferenceClientModule(argc, argv, "BoxParker"),
            m_foundGaps() {}

        BoxParker::~BoxParker() {}

        void BoxParker::setUp() {
            // This method will be call automatically _before_ running body().
        }

        void BoxParker::tearDown() {
            // This method will be call automatically _after_ return from body().
        }

        vector<double> BoxParker::getFoundGaps() const {
            return m_foundGaps;
        }
        
        
        int stageMeasuringFun ();

        // This method will do the main data processing job.
        odcore::data::dmcp::ModuleExitCodeMessage::ModuleExitCode BoxParker::body() {
            double distanceOld = 0;
            double absPathStart = 0;
            double absPathEnd = 0;

            int stageMoving = 0;
            int stageMeasuring = 0;
            
            int stageTurning = 0;
            int stageHardMove= 0;
            int stateMachine= 0;
            //bool hardCodedTravel= false;
              
             
              
              
              

            while (getModuleStateAndWaitForRemainingTimeInTimeslice() == odcore::data::dmcp::ModuleStateMessage::RUNNING) {
                // 1. Get most recent vehicle data:
                Container containerVehicleData = getKeyValueDataStore().get(automotive::VehicleData::ID());
                VehicleData vd = containerVehicleData.getData<VehicleData> ();

                // 2. Get most recent sensor board data describing virtual sensor data:
                Container containerSensorBoardData = getKeyValueDataStore().get(automotive::miniature::SensorBoardData::ID());
                SensorBoardData sbd = containerSensorBoardData.getData<SensorBoardData> ();

                // Create vehicle control data.
                VehicleControl vc;
                // My Code --------------------------------------------------------------------------------------------------------------------------------------------- 
                
                 
                switch (stateMachine) { 
                
                case 0: 
                // Hard coded movement 
                {
                           
                           if  (vd.getAbsTraveledPath() < 30) {
                           vc.setSpeed(2);    
                           vc.setSteeringWheelAngle(0);  
                          
                           }else {
	                            if ((stageHardMove >= 0) && (stageHardMove < 20)) {
			                    vc.setSpeed(0);
			                    vc.setSteeringWheelAngle(0);
			                    stageHardMove++;
			           }
	                            if ((stageHardMove >= 20) && (stageHardMove < 80)) {
			                    vc.setSpeed(2);
			                    vc.setSteeringWheelAngle(25);
			                    stageHardMove++;
		                  }
		                       if ((stageHardMove >= 80) && (stageHardMove < 94)) {
			                    vc.setSpeed(1);
			                    vc.setSteeringWheelAngle(-25);
			                    stageHardMove++;
		                  }
		                           if ((stageHardMove >= 94) && (stageHardMove < 100)) {
			                    vc.setSpeed(0);
			                    vc.setSteeringWheelAngle(0);
			                    stageHardMove++;
			                    
		                  }		                           
		                           if ((stageHardMove >=100) && (stageHardMove < 180)) {
			                    vc.setSpeed(1);
			                    vc.setSteeringWheelAngle(0);
			                    stageHardMove++;
		                  }
		                           if ((stageHardMove >= 180) && (stageHardMove < 257)) {
			                    vc.setSpeed(1);
			                    vc.setSteeringWheelAngle(-25);
			                    stageHardMove++;
			                    
		                 }
		                           if ((stageHardMove >= 257) && (stageHardMove < 275)) {
			                    vc.setSpeed(1);
			                    vc.setSteeringWheelAngle(0);
			                    stageHardMove++;
			                    
		                  }
		                           // Go to Turnining right at up side
		                           if (stageHardMove >= 275)  {
		                           vc.setSpeed(0);
			                    vc.setSteeringWheelAngle(0);
		                            stageHardMove= 0;
		                             stateMachine= 1;
			                    
			     	                    
		                  }
                    }      
                }
                break;
                case 1: 
                // Turning right at up side 
                {
                                   
                                  if ((stageTurning >= 0) && (stageTurning < 168)) {
		                    // Move slightly forward.
		                    vc.setSpeed(1);
		                    vc.setSteeringWheelAngle(25);
		                    stageTurning++;
		                }		                
		                   if ((stageTurning >= 168) && (stageTurning < 175)) {
		                    // Stop.
		                    vc.setSpeed(0);
		                    vc.setSteeringWheelAngle(0);
		                    stageTurning++;
		                 }
		                      if (stageTurning >= 175) {
		                    // End component.
		                     stateMachine= 2;
		                } 
		                
                }
                break;
                case 2:
                // Searching for empty spot and parking
                {
	                        // Moving state machine.
		                if (stageMoving == 0) {
		                    // Go forward.
		                    vc.setSpeed(1);
		                    vc.setSteeringWheelAngle(0);
		                }
		                if ((stageMoving > 0) && (stageMoving < 15)) {
		                    // Move slightly forward.
		                    vc.setSpeed(1);
		                    vc.setSteeringWheelAngle(0);
		                    stageMoving++;
		                }
		                if ((stageMoving >= 15) && (stageMoving < 25)) {
		                    // Stop.
		                    vc.setSpeed(0);
		                    vc.setSteeringWheelAngle(0);
		                    stageMoving++;
		                }
		                if ((stageMoving >= 25) && (stageMoving < 80)) {
		                    // Backwards, steering wheel to the right.
		                    vc.setSpeed(-1.85);
		                    vc.setSteeringWheelAngle(25);
		                    stageMoving++;
		                }
		                if ((stageMoving >= 80) && (stageMoving < 85)) {
		                    // Stop.
		                    vc.setSpeed(1);
		                    vc.setSteeringWheelAngle(0);
		                    stageMoving++;
		                }
		                    if (stageMoving >= 85) {
		                    // Stop.
		                    vc.setSpeed(0);
		                    vc.setSteeringWheelAngle(0);
		                    stageMoving++;
		                }
		                if (stageMoving >= 150) {
		                    // End component.
		                    break;
		                }
	                
	                //  // Measuring state machine.
		        switch (stageMeasuring) {
	                    case 0:
	                        {
	                            // Initialize measurement.
	                            distanceOld = sbd.getValueForKey_MapOfDistances(2);
	                            stageMeasuring++;
	                        }
	                    break;
	                    case 1:
	                        {
	                            // Checking for distance sequence +, -.
	                            if ((distanceOld > 0) && (sbd.getValueForKey_MapOfDistances(2) < 0)) {
	                                // Found distance sequence +, -.
	                                stageMeasuring = 2;
	                                absPathStart = vd.getAbsTraveledPath();
	                            }
	                            distanceOld = sbd.getValueForKey_MapOfDistances(2);
	                        }
	                    break;
	                    case 2:
	                        {
	                            // Checking for distance sequence -, +.
	                            if ((distanceOld < 0) && (sbd.getValueForKey_MapOfDistances(2) > 0)) {
	                                // Found distance sequence -, +.
	                                stageMeasuring = 1;
	                                absPathEnd = vd.getAbsTraveledPath();

	                                const double GAP_SIZE = (absPathEnd - absPathStart);
	                                cerr << "Size = " << GAP_SIZE << endl;
	                                m_foundGaps.push_back(GAP_SIZE);
	                                

	                                if ((stageMoving < 1) && (GAP_SIZE > 3.5)) {
	                                    stageMoving = 1;
	                                }else 
	                                {
		                                 if (sbd.getValueForKey_MapOfDistances(4) <0 ) 
		                                 {
		                                     stageTurning= 0;		                                     
		                                     if  (((vd.getHeading() >4) && (vd.getHeading() <5)) || ((vd.getHeading() <-1) && (vd.getHeading() > -2) )) {stateMachine= 3;} 
		                                     else if (((vd.getHeading() >1) && (vd.getHeading() <2) )) {stateMachine= 1;}
		                                     if  (vd.getAbsTraveledPath() > 175) { cout << "There is no empty spot at this parking lot "<< endl; stateMachine= 4; }                          		                                    	 	                                     
		                                 }
	                                
	                                } 
	                            }
	                            distanceOld = sbd.getValueForKey_MapOfDistances(2);
	                            
	                        }
	                    break;
                     }// switch (stageMeasuring)	                	                
                }// swirch case 2
                break;
                case 3: 
                // Turning at bottom for searching the other line 
                {            
		                       
		                 if ((stageTurning >= 0) && (stageTurning < 75)) {
		                    // Move slightly forward.
		                    vc.setSpeed(1);
		                    vc.setSteeringWheelAngle(-25);
		                    stageTurning++;
		                }
		                if ((stageTurning >= 75) && (stageTurning < 85)) {
		                    // Stop.
		                    vc.setSpeed(0);
		                    vc.setSteeringWheelAngle(0);
		                    stageTurning++;
		                }
		                if ((stageTurning >= 85) && (stageTurning < 140)) {
		                    // Backwards, steering wheel to the right.
		                    vc.setSpeed(-1);
		                    vc.setSteeringWheelAngle(0);
		                    stageTurning++;
		                 }
		                    if ((stageTurning >= 140) && (stageTurning < 150)) {
		                    // Stop.
		                    vc.setSpeed(0);
		                    vc.setSteeringWheelAngle(0);
		                    stageTurning++;
		                 }
		                   if ((stageTurning >= 150) && (stageTurning < 238)) {
		                    // Backwards, steering wheel to the right.
		                    vc.setSpeed(1);
		                    vc.setSteeringWheelAngle(-25);
		                    stageTurning++; 
		                 
		                 }
		                    if ((stageTurning >= 238) && (stageTurning < 250)) {
		                    // Stop.
		                    vc.setSpeed(0);
		                    vc.setSteeringWheelAngle(0);
		                    stageTurning++;
		                 }
		                      if (stageTurning >= 250) {
		                    // End component.
		                    stateMachine= 2;
		                }                                                       
              }// Case 3
              break;   
              case 4: 
              // No empty spot is found on this parking lot 
              {
                              
                                vc.setSpeed(0);
			        vc.setSteeringWheelAngle(0);
			        
              }
              break;
        } // Outer Case                           
             
              
             
        
                // Create container for finally sending the data.
                Container c(vc);
                // Send container.
                getConference().send(c);
            }

            return odcore::data::dmcp::ModuleExitCodeMessage::OKAY;
        }
    } // miniature
} // automotive
   
