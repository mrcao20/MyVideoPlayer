
// 参数：待加密文本;返回：加密文本
function EncryptText(e) {
	for (var t = [], r = 0; r < 64;) t[r] = 0 | 4294967296 * Math.abs(Math.sin(++r));
	function i(e, t) {
		return ((e >> 1) + (t >> 1) << 1) + (1 & e) + (1 & t)
	}
	for (var r, n, a, o, s = [], d = unescape(encodeURI(e)), l = d.length, u = [r = 1732584193, n = -271733879, ~r, ~n], c = 0; c <= l;) s[c >> 2] |= (d.charCodeAt(c) || 128) << c++%4 * 8;
	for (s[e = 16 * (l + 8 >> 6) + 14] = 8 * l, c = 0; c < e; c += 16) {
		for (l = u, o = 0; o < 64;) l = [a = l[3], i(r = l[1], (a = i(i(l[0], [r & (n = l[2]) | ~r & a, a & r | ~a & n, r ^ n ^ a, n ^ (r | ~a)][l = o >> 4]), i(t[o], s[[o, 5 * o + 1, 3 * o + 5, 7 * o][l] % 16 + c]))) << (l = [7, 12, 17, 22, 5, 9, 14, 20, 4, 11, 16, 23, 6, 10, 15, 21][4 * l + o++%4]) | a >>> 32 - l), r, n];
		for (o = 4; o;) u[--o] = i(u[o], l[o])
	}
	for (e = ""; o < 32;) e += (u[o >> 3] >> 4 * (1 ^ 7 & o++) & 15).toString(16);
	return e
}

// 参数顺序：platform，vid，sdtfrom，1，tm，encryptVer;返回：加密文本
function Encrypt(e, t, r, n, a, o) {
	var s = "123456";
	if (o.length < 3) return "err";
	if ("7." != o.substr(0, 2)) return "err";
	var d = o.substr(2);
	"1" == d && (s = "06fc1464"),
	"2" == d && (s = "4244ce1b"),
	"3" == d && (s = "77de31c5"),
	"4" == d && (s = "e0149fa2"),
	"5" == d && (s = "60394ced"),
	"6" == d && (s = "2da639f0"),
	"7" == d && (s = "c2f0cf9f");
	a = a || parseInt( + new Date / 1e3),
	n = ("" + n).charAt(0);
	return EncryptText(s + t + a + "*#06#" + e)
}
