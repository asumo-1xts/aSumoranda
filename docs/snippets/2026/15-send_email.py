import os
import smtplib
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import datetime


def create_email_content():
    """
    メールの件名と本文を生成する関数
    """
    # メール件名
    subject = f"お買い上げありがとうございます [1x telescope]"
    # メール本文は別添えのHTMLファイルから読み込む
    with open(
        os.path.join(os.path.dirname(__file__), "./email.html"), "r", encoding="utf-8"
    ) as f:
        html_content = f.read()

    return subject, html_content


def send_email(subject, html_content):
    """メールを送信する関数"""
    # 環境変数から顧客メールアドレスを取得
    customer_email = os.environ.get("PAYLOAD_EMAIL")
    if not customer_email:
        print("No payload email found.")
        return

    smtp_server = "smtp.gmail.com"
    smtp_port = 587
    smtp_username = "1xtelescope@gmail.com"
    smtp_password = os.environ.get("SMTP_PASSWORD")

    msg = MIMEMultipart("alternative")
    msg["Subject"] = subject
    msg["From"] = smtp_username
    msg["To"] = customer_email
    msg.attach(MIMEText(html_content, "html", "utf-8"))

    try:
        with smtplib.SMTP(smtp_server, smtp_port) as server:
            server.starttls()
            server.login(smtp_username, smtp_password)
            server.send_message(msg)
        print("Success to send email.")
    except Exception as e:
        print(f"Error: Failed to send email - {e}")


if __name__ == "__main__":
    mail_subject, mail_body = create_email_content()
    send_email(mail_subject, mail_body)