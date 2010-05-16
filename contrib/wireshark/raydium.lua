-- WIP !!!
-- Very VERY experimental Wireshark lua dissectors for Raydium 3D engine applications and servers
-- Xfennec,
-- http://raydium.org

-- You must enable lua in Wireshark (see init.lua) and call this script:
-- -X lua_script:.../raydium.lua, or dofile ".../raydium.lua" in the user init.lua)

-- "defines"
PACKET_OFFSET = 4

-- declare our protocol
raydium_proto = Proto("raydium","Raydium Network Protocol")

-- create a function to dissect it
function raydium_proto.dissector(buffer,pinfo,tree)
    pinfo.cols.protocol = "RAYDIUM"

	local type = buffer(0,1):uint()
	local tcpid=buffer(2,2):le_uint()

	local data_str="(no or unknown data)";
	local data_len=1
	local display_from=true

	if type==1 then
		type_str="DATA"
	elseif type==2 then
		type_str="ERROR_NO_MORE_PLACE"
	elseif type==3 then
		type_str="ATTRIB_UID"
	elseif type==4 then
		type_str="PACKET_REQUEST_UID"
		local name=buffer(PACKET_OFFSET):string()
		data_str="NAME: " .. name
		data_len=string.len(name)+1
		display_from=false
	elseif type==5 then
		type_str="INFO_NAME"
	elseif type==6 then
		type_str="ACK"
		data_str="ACK for " .. buffer(PACKET_OFFSET,2):le_uint()
		data_len=2
	elseif type==7 then
		type_str="SERVER_BEACON"
	elseif type==10 then
		type_str="ODE_DATA"
	elseif type==11 then
		type_str="ODE_NEWELEM"
	elseif type==12 then
		type_str="ODE_REMELEM"
	elseif type==13 then
		type_str="ODE_NIDWHO"
	elseif type==14 then
		type_str="ODE_EXPLOSION"
	elseif type>=20 then
		type_str="USER (" .. type .. ")"
	else
		type_str="(unkown type)"
	end

    local subtree = tree:add(raydium_proto,buffer(),"Raydium Network Protocol")
    subtree:add(buffer(0,1),"Packet type: " .. type_str)
	if display_from then subtree:add(buffer(1,1),"From ID: " .. buffer(1,1):uint() .. " (may be uninitialized)") end
	if tcpid~=0 then subtree:add(buffer(2,2),"TCPID: " .. tcpid) end
    subtree:add(buffer(PACKET_OFFSET,data_len),data_str)

-- For future reference ...
--    subtree = subtree:add(buffer(2,2),"The next two bytes")
--    subtree:add(buffer(2,1),"The 3rd byte: " .. buffer(2,1):uint())
end

-- load the udp.port table
udp_table = DissectorTable.get("udp.port")
udp_table:add(29104,raydium_proto)