

f = io.open("bbl.txt", "r");
f2 = io.open("bbl.bib", "w");

--[[
s = "http://asmedl.aip.org/journals/doc/ASMEDL-home/most_downloaded.jsp?KEY=JMOEEX&Year=2008&Month=12&agg=md"
print(s);
print(string.gsub(s, "&", "\&"));
]]

for line in f:lines() do 
	line = string.gsub(line, "_", "\\_");
	line = string.gsub(line, "%%", "\\%");
	line = string.gsub(line, "&",  "\\&");
	line = string.gsub(line, "^(%d+).", "@Misc{dk%1,\ntitle={ {") .. "} },}\n\n";
	print(line);
	f2:write(line);
	f2:flush();
end
