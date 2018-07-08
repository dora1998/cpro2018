1;

tbl_100 = dlmread('test0.9_ada.log', '\t', 4, 0);
tbl_050 = dlmread('test0.9_ada_0.05.log', '\t', 4, 0);
tbl_025 = dlmread('test0.9_ada_0.025.log', '\t', 4, 0);
tbl_020 = dlmread('test0.9_ada_0.02.log', '\t', 4, 0);
tbl_010 = dlmread('test0.9_ada_0.01.log', '\t', 4, 0);

tbl_100(size(tbl_100, 1), :) = [];
tbl_050(size(tbl_050, 1), :) = [];
tbl_025(size(tbl_025, 1), :) = [];
tbl_020(size(tbl_020, 1), :) = [];
tbl_010(size(tbl_010, 1), :) = [];

plot(tbl_100(:, 1), tbl_100(:, 2), 'b-@;0.1;',
    tbl_050(:, 1), tbl_050(:, 2), 'r-@;0.05;',
    tbl_025(:, 1), tbl_025(:, 2), 'g-@;0.025;',
    tbl_020(:, 1), tbl_020(:, 2), 'm-@;0.02;',
    tbl_010(:, 1), tbl_010(:, 2), 'c-@;0.01;');
title("AdaGrad");
xlabel("Epoc");
ylabel("Study Rate[%%]");
ylim([80, 100]);
grid "on"

legend('location', 'southwest');