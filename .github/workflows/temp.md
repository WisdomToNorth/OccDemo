```base
              wget https://github.com/Open-Cascade-SAS/OCCT/archive/refs/tags/V7_7_2.tar.gz
              7z x V7_7_2.tar.gz
              rm -rf V7_7_2.tar.gz
              7z x V7_7_2.tar
              rm -rf V7_7_2.tar
              cd OCCT-7_7_2
              mkdir -p build
              cd build
              cmake .. \
              -DCMAKE_BUILD_TYPE=release \
              -DCMAKE_INSTALL_RPATH="" \
              -DCMAKE_INSTALL_PREFIX=/usr \
              -DUSE_FREEIMAGE=OFF \
              -DUSE_FFMPEG=OFF \
              -DUSE_VTK=OFF \
              -DUSE_TBB=OFF
              make -j24
              sudo make install

        - name: Get count of CPU cores
          uses: SimenB/github-actions-cpu-cores@v1
          id: cpu-cores

        - name: Build
          run: |
              cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Release -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc-12 -S${{github.workspace}} -B${{github.workspace}}/build
              cmake -B build
              cmake --build ${{github.workspace}}/build --config Release --target all --
```