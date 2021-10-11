# Eden Member Check

## Description

Eden Member Check provides a quick way to get started with Eden members validation.

This project is directly related to [82ed3d0](https://github.com/eoscommunity/Eden/tree/82ed3d05892977f7c796c6ebb86a7dab5b6b3d18) version, future changes to members table structure may not work with this contract.

### Prerequisites

- [Git](https://git-scm.com/)
- [Eosio](https://developers.eos.io/welcome/latest/getting-started-guide/local-development-environment/index)

## Compilation

This contract is built with eden `clsdk` compiler, to have the local environment configured to compile this contract is needed to [take a look at this link](https://github.com/eoscommunity/demo-clsdk) to learn more about `demo-clsdk` or follow these steps to continue forward.

## Get Started

### Ubuntu 20.04

Run the following commands to set local environment.

```
sudo apt-get update
sudo apt-get install -yq    \
    binaryen                \
    build-essential         \
    cmake                   \
    gdb                     \
    git                     \
    libboost-all-dev        \
    libcurl4-openssl-dev    \
    libgmp-dev              \
    libssl-dev              \
    libusb-1.0-0-dev        \
    pkg-config              \
    wget


mkdir ~/work

cd ~/work
wget https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sdk-12.0-linux.tar.gz
tar xf wasi-sdk-12.0-linux.tar.gz

cd ~/work
wget https://github.com/eoscommunity/Eden/releases/download/sdk-v0.1.0-alpha/clsdk-ubuntu-20-04.tar.gz
tar xf clsdk-ubuntu-20-04.tar.gz
```

Make sure to have the following variables to be able to compile the `edenmember` contract:

```
export WASI_SDK_PREFIX=~/work/wasi-sdk-12.0
export CLSDK_PREFIX=~/work/clsdk

export PATH=$CLSDK_PREFIX/bin:$PATH
```

Once the previous step is completed, inside of `edenmember` folder:

```
mkdir build
cd build
cmake `clsdk-cmake-args` ..
make -j
```

## File structure

```text title="./eos-local"
.
├── CMakeLists.txt ..................... Needed minimum requirement to compile the smart contract
├── edenmember.abi ..................... Abi file that includes ricardian contracts and clauses
├── edenmember.wasm .................... Wasm file
├── include
│   ├── edenmember.hpp ................. Header file for edenmember
│   └── utils.hpp ...................... Header file for edenmember.hpp file
├── ricardian
│   └── edenmember-ricardian.cpp ....... Ricardian contracts and clauses
├── script.sh .......................... Logic to fast contract deploy
├── src
│   └── edenmember.cpp ................. Logic to header file actions
```

## License

MIT © [EOS Costa Rica](https://eoscostarica.io/)

## Contributing

If you want to contribute to this repository, please follow the steps below:

1. Fork the project
2. Create a new branch (`git checkout -b feat/sometodo`)
3. Commit changes (`git commit -m '<type>(<scope>): <subject>'`)
4. Push the commit (`git push origin feat/sometodo`)
5. Open a Pull Request

Read the EOS Costa Rica open source [contribution guidelines](https://guide.eoscostarica.io/docs/open-source-guidelines/) for more information on scheduling conventions.

If you find any bugs, please report them by opening an issue at [this link](https://github.com/eoscostarica/eden-member-check).

## What is EOSIO?

EOSIO is a highly performant open-source blockchain platform, built to support and operate safe, compliant, and predictable digital infrastructures.

## About EOS Costa Rica

<br>
<center>
<img src="https://raw.githubusercontent.com/eoscostarica/design-assets/master/logos/eosCR/fullColor-horizontal-transparent-white.png" width="400" >
</center>
<br>

EOS Costa Rica is an independently-owned, self-funded, bare-metal Genesis block producer that provides stable and secure infrastructure for EOSIO blockchains. We support open source software for our community while offering enterprise solutions and custom smart contract development for our clients.

[eoscostarica.io](https://eoscostarica.io/)
