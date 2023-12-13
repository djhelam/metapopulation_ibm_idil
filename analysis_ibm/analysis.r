rm(list = ls()) # clear workspace
extinction_probability=0 #extinction probability
dispersal_mortality=c(0,0.01,0.05,0.1,0.3,0.5,1) #dispersal mortality explored
replicates=5 #number of replicates
time_max=2000 #number of timesteps
number_of_patches=100 #number of patches
ext_names=c("ext0") #names of folders
mu_names=c("mu0","mu001","mu005","mu01","mu03","mu05","mu1")

#make a pdf to plot
pdf("disp.pdf")
par(mfrow=c(1,length(dispersal_mortality)),mar=c(0,0,0,0))
for(m in 1:length(dispersal_mortality))
{
  output=read.table(paste(ext_names,"/",mu_names[m],"/output.txt",sep=""),header=T) #read output files
  plot(0,0,type="n",ylim=c(0,1),xlim=c(0,2000)) #make a blank plot
  for(r in 0) #for only one replicate
  {
    dispersal=array() #store dispersal as a function of time
    for(t in 0:(time_max-1))
    {
      dispersal[(t+1)]=mean(output$measured_dispersal[output$r==r & output$t==t]) #mean dispersal accross all patches
    }
    lines(1:time_max,dispersal) #plot mean dispersal
  }
}
dev.off()


plot_data=data.frame() #store data required for plotting
for(m in 1:length(dispersal_mortality)) #go through all dispersal mortality
{
  output=read.table(paste(ext_names,"/",mu_names[m],"/output.txt",sep=""),header=T) #read output file
  for(r in 0:(replicates-1)) # go through all replicates
  {
    temp=data.frame(dispersal_mortality=dispersal_mortality[m],r=r,dispersal=mean(output$measured_dispersal[output$r==r & output$t==(time_max-1)])) #extract mean dispersal at the last time step
    plot_data=rbind(plot_data,temp) #append this data
  }
}
par(mfrow=c(1,1),mar=c(4,4,4,4)) # make a plot

pdf("eps0.pdf") #open pdf
#plot dispersal as a function of dispersal mortality
plot(plot_data$dispersal_mortality,plot_data$dispersal,ylim=c(0,1),pch=16,xlab="Dispersal Mortality", ylab="Evolved Dispersal Rate")

dev.off()

 