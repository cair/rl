//
// Created by per on 9/15/21.
//

#ifndef CAIRL_MICRORTS_MICRORTS_H
#define CAIRL_MICRORTS_MICRORTS_H

class MicroRTS{


public:

    void create_config(){
        if(!std::filesystem::exists("config.properties")){


            std::string buffer = "# STANDALONE = Starts MicroRTS as a standalone instance (No remote agents).\n"
                                 "# GUI = Starts the microRTS GUI.\n"
                                 "# SERVER = Starts MicroRTS as a server instance.\n"
                                 "# CLIENT = Starts MicroRTS as a client instance.\n"
                                 "launch_mode=STANDALONE\n"
                                 "\n"
                                 "################### NETWORKING ###################\n"
                                 "# Only needed if modes are SERVER/CLIENT\n"
                                 "# server_address=127.0.0.1\n"
                                 "# server_port=9898\n"
                                 "# 1 = XML\n"
                                 "# 2 = JSON\n"
                                 "# serialization_type=2\n"
                                 "\n"
                                 "# true if terrain, width and height are to be passed in every single state\n"
                                 "# false to pass that constant data only in pre-game analysis\n"
                                 "constants_in_state=true\n"
                                 "# true to compress terrain information using run-length encoding, where\n"
                                 "# 0 = A and 1 = B, false to send the full binary data\n"
                                 "compress_terrain=false\n"
                                 "##################################################\n"
                                 "\n"
                                 "# MAP\n"
                                 "map_location=maps/16x16/basesWorkers16x16.xml\n"
                                 "\n"
                                 "#### GAME SETTINGS ###\n"
                                 "\n"
                                 "# The max number of cycles the game will perform.\n"
                                 "max_cycles=5000\n"
                                 "\n"
                                 "# update interval, in milliseconds\n"
                                 "# ignored if headless == true\n"
                                 "update_interval=5\n"
                                 "\n"
                                 "# in headless mode, no window is drawn when running the game\n"
                                 "# the game also updates without intervals, which is much faster\n"
                                 "headless=true\n"
                                 "\n"
                                 "# If false, the players have full vision of the map.\n"
                                 "partially_observable=false\n"
                                 "\n"
                                 "# Versions of the Unit Type Table (DEFAULT = 2)\n"
                                 "# 1 = original\n"
                                 "# 2 = original finetuned\n"
                                 "# 3 = non-deterministic version of original finetuned (damages are random)\n"
                                 "UTT_version=2\n"
                                 "\n"
                                 "# Conflict policies (DEFAULT = 1)\n"
                                 "# 1 = A conflict resolution policy where move conflicts cancel both moves\n"
                                 "# 2 = A conflict resolution policy where move conflicts are solved randomly\n"
                                 "# 3 = A conflict resolution policy where move conflicts are solved by alternating the units trying to move\n"
                                 "conflict_policy=1\n"
                                 "\n"
                                 "# In STANDALONE mode, set which AIs to play\n"
                                 "# in CLIENT mode, AI1 will always be created via socket,\n"
                                 "# but AI2 wil be used as the AI that will play against AI1\n"
                                 "AI1=ai.abstraction.WorkerRush\n"
                                 "AI2=ai.RandomBiasedAI";


            std::ofstream f("config.properties");
            f.write(buffer.data(), buffer.size());
        }
    }
    Java java;
    Class cls;

    // Still needs abit work...
    MicroRTS(){

        create_config();
        java = Java({
             "/usr/bin/java",
             "MicroRTS-1.0.0.jar"
        });
        cls = java.getClass("rts/MicroRTS");
    }

    const cv::Mat& render(const char* = "human"/*mode*/) override{
        // TODO not yet implemented...
        return dummy;
    }


    StepReturnType step(ActionType a) override{
        // TODO not yet implemented.

        cls.main({
            "-f",
            "config.properties"
        });

        return std::tie(state, reward, terminal, info);
    }

    StateType& reset() override{
        // TODO not yet implemented
        return state;
    }



};



#endif //CAIRL_MICRORTS_MICRORTS_H
