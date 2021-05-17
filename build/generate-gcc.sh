
. ./.generate.sh

generate_all gcc -DGUIPP_STANDALONE_BUILD=ON -DGUIPP_USE_XCB=ON $*
#generate gcc-Debug Debug -DGUIPP_STANDALONE_BUILD=ON -DGUIPP_USE_XCB=ON $*
