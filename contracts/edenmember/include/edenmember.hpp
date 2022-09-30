#pragma once

/*
 * @file
 * @author  (C) 2021 by edenia [ https://edenia.com ]
 * @version 1.1.0
 *
 * @section DESCRIPTION
 *
 *    Smart contract edenmember 
 *
 *    GitHub:         https://github.com/edenia/eden-member-check
 *
 */

#include <eosio/eosio.hpp>
#include <eosio/permission.hpp> 
#include "utils.hpp"
#include "../ricardian/edenmember-ricardian.cpp"

using namespace std;
using namespace eosio;
using eosio::public_key;

namespace eosio {
    constexpr name eden_account{"genesis.eden"_n};

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
    EOSIO_REFLECT(member_v0, account, name, status, nft_template_id)

    // - A member can donate at any time after the end of a scheduled election and before
    //   the start of the next scheduled election.
    // - A member who does not make a donation before the election starts will be deactivated.
    //
    struct member_v1 : member_v0
    {
    };
    EOSIO_REFLECT(member_v1,
                    base member_v0,
                    election_participation_status,
                    election_rank,
                    representative,
                    encryption_key);

    using member_variant = std::variant<member_v0, member_v1>;

    struct member
    {
        member_variant value;
        EDEN_FORWARD_MEMBERS(value,
                            account,
                            name,
                            status,
                            nft_template_id,
                            election_participation_status,
                            election_rank,
                            representative,
                            encryption_key);
        EDEN_FORWARD_FUNCTIONS(value, primary_key, by_representative)
    };
    EOSIO_REFLECT(member, value)
    using member_table_type = eosio::multi_index<"member"_n, member>;

    bool is_eden(name account) {
        member_table_type member_tb(eden_account, 0);
        auto it = member_tb.find(account.value);
        if(it==member_tb.end() || !it->status()) return false;
        else return true;
    }
} // namespace eosio

namespace edenia {
    // This table is only for demostration purpose
    struct member {
        name user;
        uint64_t primary_key() const { return user.value; }
    };
    EOSIO_REFLECT(member, user)
    typedef eosio::multi_index<"member"_n, member> member_table;

    struct edenmember  : public eosio::contract {
        using eosio::contract::contract;
        
        /**
        *
        *  Check if given name is an Eden member to add it to the Eden table
        *
        * @param user - Eden member name
        *
        * @memo the account name specific in user parameter is the ram payor
        */
        void addmember(name user);
    };

    EOSIO_ACTIONS(edenmember,
                "edenmember"_n,
                action(addmember, user, ricardian_contract(addmember_ricardian)))
                 
} // namespace edenia