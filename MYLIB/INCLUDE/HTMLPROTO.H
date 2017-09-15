#ifndef HTMLPROTO_H
#define HTMLPROTO_H

#define vbCrLf "\r\n"


class HTMLProto {
public:

AnsiString HTML(AnsiString hd ,AnsiString  bdy ) {
  return AnsiString("<HTML>") + vbCrLf + HEAD(hd) + vbCrLf + BODY(bdy) + vbCrLf + "</HTML>"
 ; }

AnsiString BODY(AnsiString txt ) {
return AnsiString("<BODY>") + vbCrLf + txt + vbCrLf + "</BODY>"
 ; }

AnsiString HEAD(AnsiString txt ) {
  return AnsiString("<HEAD>") + vbCrLf +
  "<META HTTP-EQUIV=""Content-Type"" CONTENT=""text/html; charset=windows-1251"">" + vbCrLf +
  "<TITLE>" + txt + "</TITLE>" + vbCrLf + "</HEAD>"
; }

AnsiString TITLE(AnsiString txt ) {
  return AnsiString("<TITLE>") + txt + "</TITLE>" + vbCrLf
; }

AnsiString H(AnsiString txt , int level) {
  return AnsiString("<H") + level + " > " + txt + "</H" + level + ">" + vbCrLf
; }

AnsiString TABLE(AnsiString txt, AnsiString param ="") {
  return AnsiString("<TABLE") + param + ">" + txt + "</TABLE>" + vbCrLf
; }

AnsiString ROW(AnsiString txt ,  AnsiString param="") {
  return AnsiString("<TR") + param + ">" + txt + "</TR> " + vbCrLf
; }

AnsiString CELL(AnsiString txt,  AnsiString param="") {
  return AnsiString("<TD") + param + ">" + txt + "</TD> "
; }

AnsiString HCELL(AnsiString txt, AnsiString param ="") {
  return AnsiString("<TH") + param + ">" + txt + "</TH> "
; }


AnsiString TCAPTION(AnsiString txt, AnsiString param ="") {
  return AnsiString("<CAPTION") + param + ">" + txt + "</CAPTION> " + vbCrLf
; }

AnsiString lbl(AnsiString txtlbl ,AnsiString name) {
  return AnsiString("<A NAME=""") + txtlbl + """>" + name + "</A> "
; }

AnsiString HREF(AnsiString txtURL , AnsiString name ) {
  return AnsiString("<A HREF=""") + txtURL + """>" + name + "</A> "
; }

AnsiString b( AnsiString txt) {
  return AnsiString("<B>") + txt + "</B>"
; }

AnsiString i( AnsiString txt) {
  return AnsiString("<I>") + txt + "</I>"
; }

AnsiString UL(AnsiString txt ) {
  return AnsiString("<UL>") + txt + "</UL>" + vbCrLf
; }

AnsiString LI(AnsiString txt) {
  return AnsiString("<LI>") + txt + vbCrLf
; }

AnsiString HR() {
  return AnsiString("<HR>") + vbCrLf
; }

AnsiString P(AnsiString txt) {
  return AnsiString("<P>") + txt + vbCrLf
; }

AnsiString Plain(AnsiString txt ) {
  return AnsiString("<PLAINTEXT>") + txt + "</PLAINTEXT>" + vbCrLf
; }

};


#endif