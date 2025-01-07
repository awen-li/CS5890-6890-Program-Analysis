

export ROOT=`pwd`
export target=libxml2-2.13.0
Action=$1

function wllvm_compile ()
{
	export CC="wllvm -O0 -Xclang -disable-O0-optnone -g -fno-discard-value-names -w" 
	export CXX="wllvm++ -O0 -Xclang -disable-O0-optnone -g -fno-discard-value-names -w"
	
	cd $target
	./configure --enable-shared=no
	make
	cd -

	extract-bc $target/xmllint
	ls -l $target/xmllint*

	cp $target/xmllint.bc ./
}



function compile ()
{
	export PKG_CONFIG_PATH=/root/anaconda3/lib/pkgconfig:$PKG_CONFIG_PATH
	
	if [ ! -d "$ROOT/$target" ]; then
		tar -xvf $target.tar.xz
	fi

	wllvm_compile
	
	unset CC
	unset CXX
}

if [ "$Action" == "clean" ]; then
	rm -rf $target
	rm -rf *.bc	
	exit 0
fi

cd $ROOT
compile

cd $ROOT
