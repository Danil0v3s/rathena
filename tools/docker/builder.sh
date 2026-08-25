if [ ! -f /rathena/login-server ]; then
  export runBuild=1;
elif [ ! -f /rathena/char-server ]; then
  export runBuild=1;
elif [ ! -f /rathena/map-server ]; then
  export runBuild=1;
else
  export runBuild=0;
fi

if [ "${runBuild}" -eq "1" ]; then
  ### configure once; re-run cmake yourself when you change BUILDER_CMAKE_ARGS
  if [ ! -f /rathena/build-make/CMakeCache.txt ]; then
    echo "Warning: cmake will be executed with provided values";
    echo "Make sure you have set the variables you want in the docker-compose.yml file";
    echo $BUILDER_CMAKE_ARGS
    cmake --preset make $BUILDER_CMAKE_ARGS
  fi

  cmake --build --preset make --parallel --clean-first --target login-server char-server map-server generate-import-files;
fi
