#!/bin/bash

deploy() {
    cleos -u https://jungle3.cryptolions.io set contract <someaccount> ./ -p <someaccount>@active
}

deploy