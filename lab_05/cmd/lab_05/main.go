package main

import (
	"bufio"
	"flag"
	"fmt"
	"log"
	"os"
	"strings"
	"time"

	"github.com/go-co-op/gocron"
	"gopkg.in/gomail.v2"
)

var (
	to, from, pass string
	hasAttachment  bool
)

func init() {
	flag.StringVar(&to, "t", "somereceiveremail", "receiver's email")
	flag.StringVar(&from, "f", "somesenderemail", "sender's email")
	flag.StringVar(&pass, "p", "somesenderpass", "sender's password")
	flag.BoolVar(&hasAttachment, "a", false, "attach file")
}

func main() {
	flag.Parse()

	var (
		interval            int
		attachment, message string
	)

	fmt.Print("Input sending interval (in seconds): ")
	fmt.Scanf("%d", &interval)
	fmt.Print("Input message: ")
	reader := bufio.NewReader(os.Stdin)
	message, _ = reader.ReadString('\n')
	message = strings.Replace(message, "\n", "", -1)
	m := formBaseMail(from, to, message)
	if hasAttachment {
		fmt.Print("Input attachment's name (abs path or located in same directory as binary): ")
		fmt.Scanf("%s", &attachment)
		m.Attach(attachment)
	}

	d := gomail.NewDialer("smtp.mail.ru", 465, from, pass)

	s := gocron.NewScheduler(time.UTC)
	s.Every(interval).Seconds().Do(func() {
		if err := d.DialAndSend(m); err != nil {
			log.Fatalln(err)
		}
	})
	fmt.Println("Interval sending started. Press CTRL+C to exit")
	s.StartBlocking()
}

func formBaseMail(from, to, body string) *gomail.Message {
	m := gomail.NewMessage()

	m.SetHeader("From", from)
	m.SetHeader("To", to)
	m.SetHeader("Subject", "Test mail")
	m.SetBody("text/plain", body)

	return m
}
