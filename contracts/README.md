# Eden Member Check Smart Contract

This Smart Contract has been updated to follow 3 possible approaches to validate if an account is an Active Eden Member or not.

## First Approach

Use this repository code base as reference copying the `utils.hpp` file content and create the table as it is normally done when interacting with other tables' contracts and follow the same structure described in the `edenmember.hpp` file.

### Example Code

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

> Note: It requires to be compiled with the `CLSDK`, more info [here](https://gofractally.github.io/contract-lab/book/index.html).

## Second Approach

Currently, the `checkmember` action checks for an account allowing the flow to continue as long as the account is an Active Eden Member account otherwise, the action will fail with an error when:

1. The account is not an existing EOS Account: `Account does not exist.`.
2. The account is not an Active Eden Member: `Given account is not an Eden Member`.

### Example Code

Here are some things to consider:

- `get_self()`: The contract you are developing.
- `account`: The account to check.
- You need to add the `eosio.code` permission to your `active` signature.

```c++
eosio::action{ { get_self(), "active"_n }, // permission
                   "isedenmember"_n, // contract
                   "checkmember"_n, // action
                   std::tuple{ account } }
        .send();
```

> The drawback with this solution is if the querying contract wants to have another effect when the action fails it won't be possible since the flow will break out.

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
- alice checks internally if pip is an Active Eden Member account and put the result in the `resultiseden` action as follows: `resultiseden(bob, pip, true)`.
- bob should be listening for notifications in the `resultiseden`. If the third parameter in the `resultiseden` action is `true`, then, the account is an Active Eden Member, otherwise, the account is not an Eden Account or it is an inactive Eden Member.

### Example Code

Here are some things to consider:

- `account_to_notify`: The contract you are developing. Mostly, you are going to be using the current `account` where your contract is deployed.
- `account_to_review`: The account to validate.
- You need to add the `eosio.code` permission to your `active` signature.

```c++
// your action
void < namespace >::< contract_class >::customaction( some_parameters... ) {
    // validations
    // logic

    eosio::name account_to_notify = get_self();
    eosio::name account_to_review = "someaccount";

    eosio::action{ { get_self(), "active"_n }, // permission
                   "isedenmember"_n,           // contract
                   "isedenmember"_n,           // action
                   std::tuple{ account_to_notify, account_to_review } }
        .send();
}

// listener action
void < namespace >::< contract_class >::notify_resultiseden( name account_to_notify,
                                                     name account_to_review,
                                                     bool is_eden ) {
    // logic
}
```

> To implement a listener action, take a look at the `clsdk` documentation in the [Notifications](https://gofractally.github.io/contract-lab/book/contract/notify/index.html) section or you can also use as reference the [v2.1 developer guide](https://developers.eos.io/welcome/v2.1/smart-contract-guides/payable-actions/#deposit) if you are working with default `.cdt`.
