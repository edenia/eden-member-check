<div align="center">
	<a href="https://edenia.com">
		<img src="https://raw.githubusercontent.com/edenia/.github/master/.github/workflows/images/edenia-logo.png" width="300" alt="Edenia Logo">
	</a>
	
![](https://img.shields.io/github/license/edenia/eden-member-check) 
![](https://img.shields.io/badge/code%20style-standard-brightgreen.svg) 
![](https://img.shields.io/badge/%E2%9C%93-collaborative_etiquette-brightgreen.svg) 
![](https://img.shields.io/twitter/follow/edeniaWeb3.svg?style=social&logo=twitter) 
![](https://img.shields.io/github/forks/edenia/eden-member-check?style=social)

</div>

# Eden Member Check

This project provides a quick way to get started integrating the logic to validate if an EOS account is an active Eden Member. It is meant to be used as an example to help other projects integrate features for eden members. This same code has been implemented on [eosrate.io](https://eosrate.io) for this purpose.

## EOS Mainnet Contract

The contract is deployed to [`isedenmember`](https://bloks.io/account/isedenmember) account on  the EOS mainnet.

## Getting Started

### Important Note:
This project is directly related to edenOS version [82ed3d0](https://github.com/eoscommunity/Eden/tree/82ed3d05892977f7c796c6ebb86a7dab5b6b3d18), future changes to members table structure may not work with this contract.


### Pre-Requisites

- [Git](https://git-scm.com/)
- [Docker](https://docker.com/)
- [Antelope Leap or EOSIO](https://developers.eos.io/welcome/latest/getting-started-guide/local-development-environment/index)

### Quick start

To build execute `./deploylocal.sh`

## Compilation

This contract is built with a custom complier named `clsdk` developed by the edenOS team. To configure your local environment to compile this contract you will need to [take a look at this repo](https://github.com/eoscommunity/demo-clsdk) to learn more about `demo-clsdk` and follow these steps to continue forward.

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
├── CMakeLists.txt ..................... Minimum requirements needed to compile the smart contract
├── edenmember.abi ..................... ABI file that includes ricardian contracts and clauses
├── edenmember.wasm .................... Web Assembly file
├── include
│   ├── edenmember.hpp ................. Header file for edenmember
│   └── utils.hpp ...................... Header file for edenmember.hpp file
├── ricardian
│   └── edenmember-ricardian.cpp ....... Ricardian contracts and clauses
├── script.sh .......................... Script to easily deploy the contract
├── src
│   └── edenmember.cpp ................. Contract Logic and Actions
```

## How does CLSDK works with Eden Members table
It is necessary to have the CLSDK installed to get it working because some of the next definitions are specified right there.

### Definitions
To read Eden Members table is needed to first have a small understanding of some basic function or definition of Eden contract use.

#### EDEN_FORWARD_MEMBERS
It is C++ definition that allow to specify and indicate which fields belongs to the specified struct, here in [member struct](https://github.com/edenia/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L82) we can check that `struct member` has a value of `member_variant` which represents the `member_v0` or `member_v1` fields.

#### EDEN_FORWARD_FUNCTIONS
It is as similar as [EDEN_FORWARD_MEMBERS](#eden_forward_members), but It's purpose is for [struct functions](https://github.com/edenia/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L55-L59) instead of fields.

#### EOSIO_REFLECT
Indicate to the serializer which fields to handle, and clsdk's ABI generator which fields to include, as It happens in [member_v0](https://github.com/edenia/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L61), [member_v1](https://github.com/edenia/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L70) and [member](https://github.com/edenia/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L93).

#### EOSIO_ACTIONS
Receive the contract class, contract account and the list of [action](https://github.com/edenia/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L127) that are going to be needed by the serializer.

#### EOSIO_ACTION_DISPATCHER
Take the namespace as input to get the actions that were defined into the [namespace](https://github.com/edenia/eden-member-check/blob/main/contracts/edenmember/src/edenmember.cpp#L18) scope.

#### EOSIO_ABIGEN
Last step is to generate the [abi](https://github.com/edenia/eden-member-check/blob/main/contracts/edenmember/src/edenmember.cpp#L20) according to the before configuration, so then we need to indicate the actions, tables that are going to be public, and the Ricardian Clauses.

Once that is understood, let's jump into some small snippet code examples.

### Eden Table Struct

When a contract wants to read another table, first step is to have same table struct, for Eden Members the table struct is:
#### member_v0
```c++
using member_status_type = uint8_t;
enum member_status : member_status_type {
    pending_membership = 0,
    active_member = 1
};

using election_participation_status_type = uint8_t;
enum election_participation_status : election_participation_status_type {
    not_in_election = 0,
    in_election = 1
};

struct member_v0
{
    eosio::name account;
    std::string name;
    member_status_type status;
    uint64_t nft_template_id;
    // Only reflected in v1
    election_participation_status_type election_participation_status = not_in_election;
    uint8_t election_rank = 0;
    eosio::name representative{uint64_t(-1)};
    std::optional<eosio::public_key> encryption_key;

    uint64_t primary_key() const { return account.value; }
    uint128_t by_representative() const
    {
        return (static_cast<uint128_t>(election_rank) << 64) | representative.value;
    }
};
```

#### member_v1
```c++
struct member_v1 : member_v0
{
};
```

With these table structs and the combination of the [definitions](#definitions) the last step is to create the necessary logic to check if a given user is an Eden Member or not which is the easy part of the smart contract.

```c++
bool is_eden(name account) {
    member_table_type member_tb(eden_account, 0);
    auto it = member_tb.find(account.value);
    if(it==member_tb.end() || !it->status()) return false;
    else return true;
}
```

## Contributing

If you want to contribute to this repository, please follow the steps below:

1. Fork the project
2. Create a new branch (`git checkout -b feat/sometodo`)
3. Commit changes (`git commit -m '<type>(<scope>): <subject>'`)
4. Push the commit (`git push origin feat/sometodo`)
5. Open a Pull Request

Read the EOS Costa Rica's open source [contribution guidelines](https://guide.eoscostarica.io/docs/open-source-guidelines/) for more information on scheduling conventions.

If you find any bugs, please report them by opening an issue at [this link](https://github.com/edenia/eden-member-check/issues).

## What is EOSIO?

EOSIO is a highly performant open-source blockchain platform, built to support and operate safe, compliant, and predictable digital infrastructures.

## About Edenia

<div align="center">

<a href="https://edenia.com">
	<img width="300" alt="Edenia Logo" src="https://raw.githubusercontent.com/edenia/.github/master/.github/workflows/images/edenia-logo.png"></img>
</a>

[![Twitter](https://img.shields.io/twitter/follow/EdeniaWeb3?style=for-the-badge)](https://twitter.com/EdeniaWeb3)
[![Discord](https://img.shields.io/discord/946500573677625344?color=black&label=Discord&logo=discord&logoColor=white&style=for-the-badge)](https://discord.gg/YeGcF6QwhP)


</div>

Edenia runs independent blockchain infrastructure and develops web3 solutions. Our team of technology-agnostic builders has been operating since 1987, leveraging the newest technologies to make the internet safer, more efficient, and more transparent.

[edenia.com](https://edenia.com/)
