#include "../include/edenmember.hpp"

namespace eoscostarica {
    void edenmember::addmember(name user) {
        require_auth(user);
        bool isEden = is_eden(user);

        member_table _member(_self, _self.value);
        auto member_itr = _member.find(user.value);
        check( member_itr == _member.end(), "Member already exist" );
        _member.emplace(user, [&](auto& row) {
            row.user = user;
        });
    }
} // namespace eoscostarica


EOSIO_ACTION_DISPATCHER(eoscostarica::actions)

EOSIO_ABIGEN(
    actions(eoscostarica::actions),
    table("member"_n, eoscostarica::member),
    ricardian_clause("datastorage", eoscostarica::datastorage_clause),
    ricardian_clause("datausage", eoscostarica::datausage_clause),
    ricardian_clause("dataownership", eoscostarica::dataownership_clause),
    ricardian_clause("datadistribution", eoscostarica::datadistribution_clause),
    ricardian_clause("datafuture", eoscostarica::datafuture_clause)
)