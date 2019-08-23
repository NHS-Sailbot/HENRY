./build/premake/premake_bootstrap gmake2
if [ "$#" -eq  "0" ]
then
	make
else
	make config="$1"
fi
