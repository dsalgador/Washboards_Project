setwd("C:/Users/Daniel/Dropbox/GitHub/Washboards_Project/Discrete approach");

data <- read.table("F_matrix.txt", sep =  ",", header = F)
plot(seq(0,1, 1/(length(data)-1) ), data, type = 'l')