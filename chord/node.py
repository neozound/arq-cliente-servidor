#!/usr/bin/python
import zmq
import random
import sys
import time
import hashlib
import json
import string

hash_table = {}
client = {}
node_name = ''
context = ''
lower_bound = ''
upper_bound = ''

def get_id():
    possible = list(string.ascii_lowercase)
    rnd = random.randint(0,len(possible))
    return str(possible[rnd])

def in_range(key):
    if upper_bound >= lower_bound:
        return (key > lower_bound) and (key <= upper_bound)
    else:
        return (key >= lower_bound) or (key < upper_bound)

def interval():
    return '({},{}]'.format(lower_bound, upper_bound)

def localOP(request):
    operation = request['type']
    if operation == 'insert':
        key = request['key']
        value = request['value']
        hash_table[key] = value
        print("Stored {} {} at {}".format(key,value,node_name))
    else:
        print("Local operation not implemented")

def handleClientRequest(request, successorSocket):
    clientId = request['id']
    clientAddress = request['answer_to']
    sc = context.socket(zmq.PUSH)
    sc.connect(clientAddress)
    clients[clientId] = sc

    key = request['key']
    if in_range(key):
        print("Key {} is mine!".format(key))
        localOP(request)
    else:
        print("Key {} is not mine, delegating...".format(key))
        successorSocket.send_json(request)

