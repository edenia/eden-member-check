#include "../include/edenmember.hpp"

namespace eoscostarica {
    void edenmember::checkmember(name account) {

        check( is_eden(account), "Given account is not a member account" );

    }
} // namespace eoscostarica


EOSIO_ACTION_DISPATCHER(eoscostarica::actions)

EOSIO_ABIGEN(
    actions(eoscostarica::actions),
    ricardian_clause("datastorage", eoscostarica::datastorage_clause),
    ricardian_clause("datausage", eoscostarica::datausage_clause),
    ricardian_clause("dataownership", eoscostarica::dataownership_clause),
    ricardian_clause("datadistribution", eoscostarica::datadistribution_clause),
    ricardian_clause("datafuture", eoscostarica::datafuture_clause)
)