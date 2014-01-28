## Look in src/visual for the visual main files
vpath %.cpp $(DEV_DIR)/src/visual

## The result of our nbody-visual compile: an executable
## named list-visual.x which will run our program
TARGET_PROJ_VISUAL := $(INSTALL_DIR)/bin/$(PROJ_NAME)-visual.x
BUILD_DIR_PROJ_VISUAL := $(BUILD_DIR)/$(PROJ_NAME)-visual

## nbody-visual.x depends on libnbody.a having been created first
$(TARGET_PROJ_VISUAL) : $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a

## Link our nbody-visual.x executable with libnbody.a
## (here is where you add any extra third-party library linkings needed)
$(TARGET_PROJ_VISUAL) : LDFLAGS += $(INSTALL_DIR)/lib/lib$(PROJ_NAME).a

## Add any more files to this list
OBJECTS_PROJ_visual := \
	$(BUILD_DIR_PROJ_VISUAL)/main.o \
	#$(BUILD_DIR_PROJ_VISUAL)/otherFiles.o \
	#$(BUILD_DIR_PROJ_VISUAL)/moreOtherFiles.o \
## Add any more files you like!

$(INSTALL_DIR)/include/$(PROJ_NAME)-visual ::
	$(MKDIR) $@
	$(RSYNC) $(DEV_DIR)/include/$(PROJ_NAME)-visual/ $@/

## Add the public include files to the list of artifacts
## to be cleaned up on a 'make clean'
ARTIFACTS += $(INSTALL_DIR)/include/$(PROJ_NAME)-visual
