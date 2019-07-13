
mydata <- read.csv(file="../dataInCsv/running-1000.csv", header = FALSE, sep=",")
# mydata <- read.csv(file="../dataInCsv/walking-1000.csv", header = FALSE, sep=",")

epsilon <- 100

order <- 3
begin <- 1
i <- order+begin

as <- list()

while (i <= 1000){
  t = mydata[begin:i,1]
  x = mydata[begin:i,2]
  y = mydata[begin:i,3]
  z = mydata[begin:i,4]
  
  m1 <- lm(x~poly(t,order))
  m2 <- lm(y~poly(t,order))
  m3 <- lm(z~poly(t,order))

##### Infinity norm
  error <- max(max(abs(m1$residuals)),max(abs(m2$residuals)),max(abs(m3$residuals)))
  
##### Eclidean norm
  # tmp_error = c()
  # j=1
  # for (j in 1:length(m1$residuals)){
  #   tmp_error[j] <- sqrt(m1$residuals[j]^2 + m2$residuals[j]^2 + m3$residuals[j]^2)
  # }
  # error <- max(tmp_error)

  
  if(error <= 100){
    fm1 = m1
    fm2 = m2
    fm3 = m3
    i = i+1
  } else{
      as[[length(as)+1]] <- list(fm1,fm2,fm3,i-1)
      begin <- i
      i <- order+begin
  }
}


##### plot
# plot(mydata$V1, mydata$V2, main="Regression ax order=5", ylab = "Acceleration(mg*2048)", xlab = "Time")
# lines(predict(as[[1]][[1]]), col="red",lwd=3)
# j=2
# for (j in 2:length(as)){
#   lines((as[[j-1]][[4]]+1):as[[j]][[4]],predict(as[[j]][[1]]), col="red",lwd=3)
# }
# lines((as[[length(as)]][[4]]+1):1000, predict(m1), col="red", lwd=3)
