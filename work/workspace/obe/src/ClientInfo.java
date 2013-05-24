class ClientInfo{
	public String sbeId;
	public String  pwd;
	public String isActive;
	public ClientInfo(String sbeId, String pwd, String isActive) {
		this.sbeId = sbeId;
		this.pwd = pwd;
		this.isActive = isActive;
	}
	public String getSbeId() {
		return sbeId;
	}
	public void setSbeId(String sbeId) {
		this.sbeId = sbeId;
	}
	public String getPwd() {
		return pwd;
	}
	public void setPwd(String pwd) {
		this.pwd = pwd;
	}
	public String getIsActive() {
		return isActive;
	}
	public void setIsActive(String isActive) {
		this.isActive = isActive;
	}
	
}