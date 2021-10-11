# Eden Member Check

## Description

Eden Member Check provides a quick way to get started with Eden Members validation.

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

## How does CLSDK works with Eden Members table
It's necessary to have installed the CLSDK to get it working because some of the next definitions are specified right there.

### Definitions
To read Eden Members table is needed to first have a small understanding of some basic function or definition of Eden contract use.

#### EDEN_FORWARD_MEMBERS
It is C++ definition that allow to specify and indicate which fields belongs to the specified struct, here in [member struct](https://github.com/eoscostarica/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L82) we can check that `struct member` has a value of `member_variant` which represents the `member_v0` or `member_v1` fields.

#### EDEN_FORWARD_FUNCTIONS
It is as similar as [EDEN_FORWARD_MEMBERS](#eden_forward_members), but It's purpose is for [struct functions](https://github.com/eoscostarica/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L55-L59) instead of fields.

#### EOSIO_REFLECT
Indicate to the serializer which fields to handle, and clsdk's ABI generator which fields to include, as It happens in [member_v0](https://github.com/eoscostarica/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L61), [member_v1](https://github.com/eoscostarica/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L70) and [member](https://github.com/eoscostarica/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L93).

#### EOSIO_ACTIONS
Receive the contract class, contract account and the list of [action](https://github.com/eoscostarica/eden-member-check/blob/main/contracts/edenmember/include/edenmember.hpp#L127) that are going to be needed by the serializer.

#### EOSIO_ACTION_DISPATCHER
Take the namespace as input to get the actions that were defined into the [namespace](https://github.com/eoscostarica/eden-member-check/blob/main/contracts/edenmember/src/edenmember.cpp#L18) scope.

#### EOSIO_ABIGEN
Last step is to generate the [abi](https://github.com/eoscostarica/eden-member-check/blob/main/contracts/edenmember/src/edenmember.cpp#L20) according to the before configuration, so then we need to indicate the actions, tables that are going to be public, and the Ricardian Clauses.

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

With these table structs and the combination of the [definitions](#definitions) the last step is to create the needed logic to check if a given user is an Eden Member or not which is the easier part of the smart contract.

```c++
bool is_eden(name account) {
    member_table_type member_tb(eden_account, 0);
    auto it = member_tb.find(account.value);
    if(it==member_tb.end() || !it->status()) return false;
    else return true;
}
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
