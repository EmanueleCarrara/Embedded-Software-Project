################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
main.obj: ../main.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/sam/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/sam/ti/ccs1240/ccs/ccs_base/arm/include" --include_path="/home/sam/Desktop/embedded/simplelink_msp432p4_sdk_3_40_01_02/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="/home/sam/Desktop/embedded/simplelink_msp432p4_sdk_3_40_01_02/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/sam/ti/ccs1240/ccs/ccs_base/arm/include/CMSIS" --include_path="/home/sam/Desktop/embedded/Workspace/Progetto" --include_path="/home/sam/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/home/sam/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="/home/sam/Desktop/embedded/simplelink_msp432p4_sdk_3_40_01_02/simplelink_msp432p4_sdk_3_40_01_02/source" --include_path="/home/sam/ti/ccs1240/ccs/ccs_base/arm/include/CMSIS" --include_path="/home/sam/ti/ccs1240/ccs/ccs_base/arm/include" --include_path="/home/sam/Desktop/embedded/simplelink_msp432p4_sdk_3_40_01_02/simplelink_msp432p4_sdk_3_40_01_02/source/third_party/CMSIS/Include" --include_path="/home/sam/Desktop/embedded/Workspace/Progetto" --include_path="/home/sam/ti/ccs1240/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --advice:power=all --define=__MSP432P401R__ --define=ccs -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: "$<"'
	@echo ' '


