from scapy.all import *

# Define custom packet for authentication
class AuthenticationPacket(Packet):
    name = "AuthenticationPacket"
    fields_desc = [
        StrField("username", ""),
        StrField("password", ""),
    ]
bind_layers(TCP, AuthenticationPacket)

# Define custom packet for balance inquiry
class BalanceInquiryPacket(Packet):
    name = "BalanceInquiryPacket"
bind_layers(TCP, BalanceInquiryPacket)

# Define custom packet for withdrawal request
class WithdrawalRequestPacket(Packet):
    name = "WithdrawalRequestPacket"
    fields_desc = [
        IntField("amount", 0),
    ]
bind_layers(TCP, WithdrawalRequestPacket)


# Authentication packet from ATM to bank server
auth_packet = IP(dst="127.0.0.1") / TCP(dport=1234) / AuthenticationPacket(username="user123", password="pass123")

# Bank server response to authentication
auth_response_packet = IP(dst="127.0.0.1") / TCP(dport=1234) / Raw(load="Authentication Successful")

# Balance inquiry packet from ATM to bank server
balance_inquiry_packet = IP(dst="127.0.0.1") / TCP(dport=1234) / BalanceInquiryPacket()

# Bank server response to balance inquiry
balance_response_packet = IP(dst="127.0.0.1") / TCP(dport=1234) / Raw(load="Available Balance: $1000")

# Withdrawal request packet from ATM to bank server
withdrawal_request_packet = IP(dst="127.0.0.1") / TCP(dport=1234) / WithdrawalRequestPacket(amount=200)

# Bank server response to withdrawal request
withdrawal_response_packet = IP(dst="127.0.0.1") / TCP(dport=1234) / Raw(load="Withdrawal Successful. Remaining Balance: $800")

# Combine the packets into a list
packets = [auth_packet, auth_response_packet, balance_inquiry_packet, balance_response_packet,
           withdrawal_request_packet, withdrawal_response_packet]

# Save the packets to a pcap file
wrpcap("bank_communication.pcap", packets)