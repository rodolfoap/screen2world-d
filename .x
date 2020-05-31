BINARY=gen-points
execute(){
	./${BINARY} test/detections.csv test/camera_matrix_1280x720.yaml
}
build(){
	mkdir -p build;
	pushd build &> /dev/null;
	[ -f Makefile ] || cmake .. -Wdev;
	make -j$(nproc); STATUS=$?
	popd &> /dev/null;
}
case "$1" in
	"")
		[ -f ${BINARY} ] || build
		execute
	;;
	b)
		build
	;;
	e)
		vi -p gen-points.cpp screen2worldK.cpp autocalib.cpp yamlgen.cpp include/autocalib.hpp include/auxmath.hpp include/yamlgen.hpp include/screen2worldK.hpp CMakeLists.txt
		rm -f ${BINARY}
		build;
		execute;
	;;
esac
