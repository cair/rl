# CAIRL_THIRD_PARTY_SOURCES
# CAIRL_THIRD_PARTY_INCLUDE_DIRS
# CAIRL_THIRD_PARTY_LINKER


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
##### Third Party - MicroRTS (Java)
#####
#############################################################################################
add_subdirectory(extern/java/microrts)
#list(APPEND CAIRL_THIRD_PARTY_LINKER)
list(APPEND CAIRL_THIRD_PARTY_SOURCES
        include/cairl/contrib/environments/java/MicroRTS.h
    )

#############################################################################################
#####
##### Third Party - Showdown 2.0
##### Still needs abit work. generally Showdownai 2.0 seems to be uncomplete. Need to investigate further.
#####
#############################################################################################
# TODO still a few hickups with the java impl.
#add_subdirectory(extern/java/showdownai)
#list(APPEND CAIRL_THIRD_PARTY_LINKER)
#list(APPEND CAIRL_THIRD_PARTY_SOURCES)


