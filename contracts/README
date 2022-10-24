# Eden Member Check Smart Contract

This Smart Contract has been updated to follow 3 possible approaches to validate if an account is an Active Eden Member or not.

## First Approach

Use this repository code base as reference copying the `utils.hpp` file content and create the table as it is normally done when reading another table contract and follow the same structure described in the `edenmember.hpp` file.

```c++
struct member_v0 {
    eosio::name        account;
    std::string        name;
    member_status_type status;
    uint64_t           nft_template_id;
    // Only reflected in v1
    election_participation_status_type election_participation_status =
        not_in_election;
    uint8_t                            election_rank = 0;
    eosio::name                        representative{ uint64_t( -1 ) };
    std::optional< eosio::public_key > encryption_key;

    uint64_t  primary_key() const { return account.value; }
    uint128_t by_representative() const {
        return ( static_cast< uint128_t >( election_rank ) << 64 ) |
                representative.value;
    }
};
```

```c++
struct member_v1 : member_v0 {};
```

```c++
struct member {
    member_variant value;
    EDEN_FORWARD_MEMBERS( value,
                            account,
                            name,
                            status,
                            nft_template_id,
                            election_participation_status,
                            election_rank,
                            representative,
                            encryption_key );
    EDEN_FORWARD_FUNCTIONS( value, primary_key, by_representative )
};
```

> It is required to be compiled with the `CLSDK`, more info [here](https://gofractally.github.io/contract-lab/book/index.html).

## Second Approach

Currently, the `checkmember` action checks for an account allowing the flow to continue as long as the account is Active Eden Member account otherwise, the action will fail with an error indicating:

1. Account is not an existing EOS Account with error `Account does not exist.`.
2. Account is not an Active Eden Member with error `Given account is not an Eden Member`.

The drawback with this solution is if the contract wants to take another effect for the account it won't be possible since the action will fail.

## Third Approach

With the [latest changes](https://github.com/edenia/eden-member-check/commit/d5080ce7e063b8883dc1fb9e591e380b6728cdd7) introduced in the contract, now it is able to validate an Active Eden Member throught Inline Actions by calling the `isedenmember` and wait for the answer in the `resultiseden` action.

The idea with this solution is to avoid the strict restriction of understanding how the `genesis.eden` contract works and the requirement of using the `CLSDK`.

The contract that checks an account should call `isedenmember` with `account_to_notify` which is the smart contract that will receive the check result and `account_to_review` that represent the account to check.

The `resultiseden` is going to be used as a bridge that carries the answer of the check. This action can only be called by the contract itself with the `<account>@code` permission.

### Flow Example

**alice**: Smart Contract that contains the logic to validate if an account is an Active Eden Member or not (_this Smart Contract_).
**bob**: Smart Contract that wants to validate an Eden Member Account.
**pip**: Eden Account to validate.

- bob calls alice with: `isedenmember(bob, pip)`.
- alice checks internally if pip is an Active Eden Member account and put the result in the `resultiseden` as follows: `resultiseden(bob, pip, true)`.
- bob should be listening for notifications in the `resultiseden`. If the third parameter in the `resultiseden` action is `true`, then, the account is an Active Eden Member, otherwise, the account is not an Eden Account or it is an inactive Eden Member.
