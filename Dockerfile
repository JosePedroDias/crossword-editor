FROM gcc:latest

RUN apt-get update
RUN apt-get -y dist-upgrade
RUN apt-get -y install gdb valgrind xxd

RUN mkdir -p /source
WORKDIR /source

#RUN apt -y install openssh-server gdbserver

# configure SSH for communication with Visual Studio 
#RUN mkdir -p /var/run/sshd

#RUN echo 'root:root' | chpasswd \
#    && sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config \
#    && sed 's@session\s*required\s*pam_loginuid.so@session optional pam_loginuid.so@g' -i /etc/pam.d/sshd

#CMD ["/usr/sbin/sshd", "-D"]

#EXPOSE 22
