setwd("C:/Users/Daniel/Dropbox/GitHub/Washboards_Project/Discrete approach");

filename = "F_matrix.txt"
#filename = "InitialRoad.txt"
data <- read.table(filename, sep =  ",", header = F)
par(mfrow = c(1,1))
#x= seq(0,length(data), 1 )
x=1:length(data)
plot(x, data, type = 'l')
datasmooth <- smooth.spline(x,data, spar = 0.2)
#plot(datasmooth, cex.lab = 1.5, type = 'l', lwd = 2, ylab = "Vertical position",xlab = "Horizontal position", main = "d=7, beta = 5, f_max = 15")

#polygon(c( 0, max(x) ),  c(0,data[x==max(x)]), col="red")
#with(smooth, polygon(x, data, col="gray"))
#polygon(head(datasmooth)$x, head(datasmooth)$y, col="gray")
y= sin(10*pi*x)+5
plot(x, y, type = 'l', ylim = c(0,15),lwd = 2, ylab = "Vertical position",xlab = "Horizontal position",cex.lab = 1.5, main = "More realistic")
