1;

tbl_sgd = dlmread('test0.9.log', '\t', 4, 0);
tbl_mom = dlmread('test0.9_m.log', '\t', 4, 0);
tbl_adagrad = dlmread('test0.9_ada.log', '\t', 4, 0);
tbl_sgd(size(tbl_sgd, 1), :) = [];
tbl_mom(size(tbl_mom, 1), :) = [];
tbl_adagrad(size(tbl_adagrad, 1), :) = [];

plot(tbl_sgd(:, 1), tbl_sgd(:, 2), 'b-@;SGD;',
    tbl_mom(:, 1), tbl_mom(:, 2), 'r-@;Momentum;',
    tbl_adagrad(:, 1), tbl_adagrad(:, 2), 'g-@;AdaGrad;');
title("Epoc10 lr=0.1*0.9^(epoc)");
xlabel("Epoc");
ylabel("Study Rate[%%]");
ylim([80, 100]);
grid "on"

legend('location', 'southwest');