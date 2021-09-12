# CAIRL_THIRD_PARTY_SOURCES
# CAIRL_THIRD_PARTY_INCLUDE_DIRS
# CAIRL_THIRD_PARTY_LINKER

#############################################################################################
#####
##### Third Party - General Files
#####
#############################################################################################
list(APPEND CAIRL_THIRD_PARTY_SOURCES
        include/cairl/contrib/environments/envs.h
)

#############################################################################################
#####
##### Third Party - Deep RTS
#####
#############################################################################################
add_subdirectory(extern/deep-rts)
list(APPEND CAIRL_THIRD_PARTY_LINKER
        DeepRTSLib
)
list(APPEND CAIRL_THIRD_PARTY_SOURCES
        include/cairl/contrib/environments/rts/DeepRTS.h
)

#############################################################################################
#####
##### Third Party - FlashRL
#####
#############################################################################################
add_subdirectory(extern/flashrl-lightspark)
list(APPEND CAIRL_THIRD_PARTY_SOURCES
        include/cairl/contrib/environments/flashrl/lightspark/LightsparkRunner.h
        include/cairl/contrib/environments/flashrl/FlashEngineRunner.h
        include/cairl/contrib/environments/flashrl/lightspark/CaiRLEngineData.h
        include/cairl/contrib/environments/flashrl/games/Multitask.h
        include/cairl/contrib/environments/flashrl/FlashEnv.h
)
list(APPEND CAIRL_THIRD_PARTY_LINKER
        lighspark_lib
)
