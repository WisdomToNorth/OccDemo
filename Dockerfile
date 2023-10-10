FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y build-essential procps curl file git && \
    sh -c "$(curl -fsSL https://raw.githubusercontent.com/Linuxbrew/install/master/install.sh)" && \
    test -d ~/.linuxbrew && eval $(~/.linuxbrew/bin/brew shellenv) && \
    test -d /home/linuxbrew/.linuxbrew && eval $(/home/linuxbrew/.linuxbrew/bin/brew shellenv) && \
    test -r ~/.profile && \
    echo "eval \$($(brew --prefix)/bin/brew shellenv)" >>~/.profile && \
    echo "export PATH=\"$(brew --prefix)/bin:$PATH\"" >>~/.profile && \
    source ~/.profile && \
    apt-get install -y qtbase5-dev qtbase5-dev-tools && \
    brew install opencascade && \
    brew install ninja

WORKDIR /app

COPY . .

RUN cmake --no-warn-unused-cli -DCMAKE_BUILD_TYPE:STRING=Release \
    -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/gcc \
    -DCMAKE_CXX_COMPILER=/usr/bin/g++ -B/app/build/Linux_Ninja_Release \
    -S/app -G Ninja && \
    cmake -B build && \
    cmake --build /app/build/Linux_Ninja_Release --config Release --target all --

WORKDIR /app/build/Linux_Ninja_Release

CMD ["ctest", "-T", "test", "-R"]