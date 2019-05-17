export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source $ATLAS_LOCAL_ROOT_BASE/user/atlasLocalSetup.sh
lsetup "lcgenv -p LCG_87 x86_64-slc6-gcc62-opt ROOT"
export ROOT_INCLUDE_PATH=`pwd`:$ROOT_INCLUDE_PATH
