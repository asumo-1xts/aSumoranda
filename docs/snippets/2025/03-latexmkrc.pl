#!/usr/bin/env perl

$latex = 'uplatex -synctex=1 -halt-on-error -interaction=nonstopmode -file-line-error %O %S';

$dvipdf     = 'dvipdfmx -V 1.6 %O -o %D %S';
$makeindex  = 'mendex -U %O -o %D %S';
$bibtex     = 'upbibtex %O %S';
$biber      = 'biber --bblencoding=utf8 -u -U --output_safechars %O %S';
$max_repeat = 5;
$pdf_mode   = 3;

# フォント関連
$ENV{TZ} = 'Asia/Tokyo';

# プレビュー関連
$pvc_view_file_via_temporary = 0;
if ($^O eq 'linux') {
    $dvi_previewer = "xdg-open %S";
    $pdf_previewer = "xdg-open %S";
} elsif ($^O eq 'darwin') {
    $dvi_previewer = "open %S";
    $pdf_previewer = "open %S";
} else {
    $dvi_previewer = "start %S";
    $pdf_previewer = "start %S";
}